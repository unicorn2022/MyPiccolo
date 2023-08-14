#pragma once

#include "runtime/resource/res_type/data/animation_clip.h"
#include "runtime/resource/res_type/data/animation_skeleton_node_map.h"
#include "runtime/resource/res_type/data/blend_state.h"
#include "runtime/resource/res_type/data/skeleton_data.h"
#include "runtime/resource/res_type/data/skeleton_mask.h"

#include <map>
#include <memory>
#include <string>

namespace MyPiccolo {
    class AnimationManager {
    private:
        static std::map<std::string, std::shared_ptr<SkeletonData>>  m_skeleton_definition_cache;    // 骨骼数据缓存
        static std::map<std::string, std::shared_ptr<AnimationClip>> m_animation_data_cache;         // 动画数据缓存
        static std::map<std::string, std::shared_ptr<AnimSkelMap>>   m_animation_skeleton_map_cache; // 动画骨骼映射缓存
        static std::map<std::string, std::shared_ptr<BoneBlendMask>> m_skeleton_mask_cache;          // 骨骼遮罩缓存

    public:
        // 加载骨骼数据
        static std::shared_ptr<SkeletonData> tryLoadSkeleton(std::string file_path);
        // 加载动画数据
        static std::shared_ptr<AnimationClip> tryLoadAnimation(std::string file_path);
        // 加载动画骨骼映射数据
        static std::shared_ptr<AnimSkelMap> tryLoadAnimationSkeletonMap(std::string file_path);
        // 加载骨骼遮罩数据
        static std::shared_ptr<BoneBlendMask> tryLoadSkeletonMask(std::string file_path);
        // 从 BlendState 中获取 BlendStateWithClipData
        static BlendStateWithClipData getBlendStateWithClipData(const BlendState& blend_state);

        AnimationManager() = default;
    };
} // namespace MyPiccolo