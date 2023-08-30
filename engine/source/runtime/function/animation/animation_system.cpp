#include "runtime/function/animation/animation_system.h"

#include "runtime/resource/res_type/data/skeleton_mask.h"

#include "runtime/function/animation/animation_loader.h"
#include "runtime/function/animation/skeleton.h"

namespace Piccolo {
    std::map<std::string, std::shared_ptr<SkeletonData>>  AnimationManager::m_skeleton_definition_cache;    // 骨骼数据缓存
    std::map<std::string, std::shared_ptr<AnimationClip>> AnimationManager::m_animation_data_cache;         // 动画数据缓存
    std::map<std::string, std::shared_ptr<AnimSkelMap>>   AnimationManager::m_animation_skeleton_map_cache; // 动画骨骼映射缓存
    std::map<std::string, std::shared_ptr<BoneBlendMask>> AnimationManager::m_skeleton_mask_cache;          // 骨骼遮罩缓存

    // 加载骨骼数据
    std::shared_ptr<SkeletonData> AnimationManager::tryLoadSkeleton(std::string file_path) {
        std::shared_ptr<SkeletonData> res;
        AnimationLoader               loader;

        auto found = m_skeleton_definition_cache.find(file_path);

        if (found == m_skeleton_definition_cache.end()) {
            res = loader.loadSkeletonData(file_path);
            m_skeleton_definition_cache.emplace(file_path, res);
        } else {
            res = found->second;
        }

        return res;
    }

    // 加载动画数据
    std::shared_ptr<AnimationClip> AnimationManager::tryLoadAnimation(std::string file_path) {
        std::shared_ptr<AnimationClip> res;
        AnimationLoader                loader;

        auto found = m_animation_data_cache.find(file_path);
        if (found == m_animation_data_cache.end()) {
            res = loader.loadAnimationClipData(file_path);
            m_animation_data_cache.emplace(file_path, res);
        } else {
            res = found->second;
        }

        return res;
    }

    // 加载动画骨骼映射数据
    std::shared_ptr<AnimSkelMap> AnimationManager::tryLoadAnimationSkeletonMap(std::string file_path) {
        std::shared_ptr<AnimSkelMap> res;
        AnimationLoader              loader;

        auto found = m_animation_skeleton_map_cache.find(file_path);
        if (found == m_animation_skeleton_map_cache.end()) {
            res = loader.loadAnimSkelMap(file_path);
            m_animation_skeleton_map_cache.emplace(file_path, res);
        } else {
            res = found->second;
        }

        return res;
    }

    // 加载骨骼遮罩数据
    std::shared_ptr<BoneBlendMask> AnimationManager::tryLoadSkeletonMask(std::string file_path) {
        std::shared_ptr<BoneBlendMask> res;
        AnimationLoader                loader;

        auto found = m_skeleton_mask_cache.find(file_path);
        if (found == m_skeleton_mask_cache.end()) {
            res = loader.loadSkeletonMask(file_path);
            m_skeleton_mask_cache.emplace(file_path, res);
        } else {
            res = found->second;
        }

        return res;
    }

    // 从 BlendState 中获取 BlendStateWithClipData
    BlendStateWithClipData AnimationManager::getBlendStateWithClipData(const BlendState& blend_state) {
        // ------------------------------------------------------------------------------------------------
        /* 加载 blend_state 中的三种数据 */
        for (auto animation_file_path : blend_state.blend_clip_file_path)
            tryLoadAnimation(animation_file_path);
        for (auto anim_skel_map_path : blend_state.blend_anim_skel_map_path)
            tryLoadAnimationSkeletonMap(anim_skel_map_path);
        for (auto skeleton_mask_path : blend_state.blend_mask_file_path)
            tryLoadSkeletonMask(skeleton_mask_path);

        // ------------------------------------------------------------------------------------------------
        /* 创建 BlendStateWithClipData */
        BlendStateWithClipData blend_state_with_clip_data;

        // ------------------------------------------------------------------------------------------------
        /* 载入基础数据 */
        // 添加 clip_count 数据
        blend_state_with_clip_data.clip_count = blend_state.clip_count;
        // 添加 blend_ratio 数据
        blend_state_with_clip_data.blend_ratio = blend_state.blend_ratio;
        // 添加 blend_clip 数据
        for (const auto& path : blend_state.blend_clip_file_path)
            blend_state_with_clip_data.blend_clip.push_back(*m_animation_data_cache[path]);
        // 添加 blend_anim_skel_map 数据
        for (const auto& path : blend_state.blend_anim_skel_map_path)
            blend_state_with_clip_data.blend_anim_skel_map.push_back(*m_animation_skeleton_map_cache[path]);

        // ------------------------------------------------------------------------------------------------
        /* 计算混合权重. 保证权重和为1 */
        // 获取 blend_mask 数据, 加载其中的骨骼数据
        std::vector<std::shared_ptr<BoneBlendMask>> blend_masks;
        for (auto& path : blend_state.blend_mask_file_path) {
            blend_masks.push_back(m_skeleton_mask_cache[path]);
            tryLoadAnimationSkeletonMap(m_skeleton_mask_cache[path]->skeleton_file_path);
        }
        // 计算骨骼中的节点个数
        size_t skeleton_bone_count = m_skeleton_definition_cache[blend_masks[0]->skeleton_file_path]->bones_map.size();
        // 添加 blend_weight 数据
        blend_state_with_clip_data.blend_weight.resize(blend_state.clip_count);
        for (size_t clip_index = 0; clip_index < blend_state.clip_count; clip_index++)
            blend_state_with_clip_data.blend_weight[clip_index].blend_weight.resize(skeleton_bone_count);
        // 对于每个骨骼, 设置每个clip的权重
        for (size_t bone_index = 0; bone_index < skeleton_bone_count; bone_index++) {
            // 计算所有clip的总权重
            float sum_weight = 0;
            for (size_t clip_index = 0; clip_index < blend_state.clip_count; clip_index++)
                if (blend_masks[clip_index]->enabled[bone_index])
                    sum_weight += blend_state.blend_weight[clip_index];

            if (fabs(sum_weight) < 0.0001f) {
                // LOG_ERROR
            }

            // 设置每个clip的权重
            for (size_t clip_index = 0; clip_index < blend_state.clip_count; clip_index++) {
                if (blend_masks[clip_index]->enabled[bone_index]) {
                    blend_state_with_clip_data.blend_weight[clip_index].blend_weight[bone_index] =
                        blend_state.blend_weight[clip_index] / sum_weight;
                } else {
                    blend_state_with_clip_data.blend_weight[clip_index].blend_weight[bone_index] = 0;
                }
            }
        }

        return blend_state_with_clip_data;
    }

} // namespace Piccolo