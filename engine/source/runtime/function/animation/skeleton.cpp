#include "runtime/function/animation/skeleton.h"

#include "runtime/core/math/math.h"

#include "runtime/function/animation/utilities.h"

namespace MyPiccolo {
    Skeleton::~Skeleton() { delete[] m_bones; }

    /**
     * @brief 通过骨骼数据, 构建骨骼树
     * @param skeleton_defination 骨骼数据
    */
    void Skeleton::buildSkeleton(const SkeletonData& skeleton_defination) {
        m_is_flat = skeleton_defination.is_flat;

        if (m_bones != nullptr) delete[] m_bones;

        if (!m_is_flat || !skeleton_defination.in_topological_order) {
            // LOG_ERROR
            return;
        }

        m_bone_count = skeleton_defination.bones_map.size();
        m_bones      = new Bone[m_bone_count];
        for (size_t i = 0; i < m_bone_count; i++) {
            // 第i个骨骼节点的数据
            const RawBone bone_definition = skeleton_defination.bones_map[i];
            // 第i个骨骼的父骨骼
            Bone* parent_bone = find_by_index(m_bones, bone_definition.parent_index, i, m_is_flat);
            // 初始化第i个骨骼
            m_bones[i].initialize(std::make_shared<RawBone>(bone_definition), parent_bone);
        }
    }
    /**
     * @brief 通过混合状态数据, 将动画数据应用到骨骼树上
     * @param blend_state 混合状态数据, 只计算第一个clip动画
     * TODO: 优化, 可以同时计算多个clip动画
    */
    void Skeleton::applyAnimation(const BlendStateWithClipData& blend_state) {
        if (!m_bones) return;

        resetSkeleton();

        /* 应用 blend_state 中, 第一个 clip 的数据*/
        for (size_t clip_index = 0; clip_index < 1; clip_index++) {
            // 当前 clip 的数据
            const AnimationClip& animation_clip = blend_state.blend_clip[clip_index];

            // 当前 clip 已经播放的时间比例
            const float phase = blend_state.blend_ratio[clip_index];

            // 当前 clip 的骨骼节点映射: index=>骨骼节点编号
            const AnimSkelMap& anim_skel_map = blend_state.blend_anim_skel_map[clip_index];

            // 当前时间相邻的两个帧, 通过插值, 得出最后的结果
            float exact_frame        = phase * (animation_clip.total_frame - 1);
            int   current_frame_low  = floor(exact_frame);
            int   current_frame_high = ceil(exact_frame);
            // 线性插值比例
            float lerp_ratio = exact_frame - current_frame_low;

            // 对每个骨骼节点, 计算平移&旋转&缩放
            for (size_t node_index = 0;
                 node_index < animation_clip.node_count && node_index < anim_skel_map.convert.size();
                 node_index++) {
                // 当前节点的动画通道
                AnimationChannel channel = animation_clip.node_channels[node_index];
                // 当前节点的对应的骨骼节点索引
                size_t bone_index = anim_skel_map.convert[node_index];
                // 权重
                float weight = 1; // blend_state.blend_weight[clip_index]->blend_weight[bone_index];

                if (fabs(weight) < 0.001f) continue;
                if (bone_index == std::numeric_limits<size_t>::max()) continue;

                // 当前帧的上界不能超过关键帧的大小
                if (channel.position_keys.size() <= current_frame_high)
                    current_frame_high = channel.position_keys.size() - 1;
                if (channel.scaling_keys.size() <= current_frame_high)
                    current_frame_high = channel.scaling_keys.size() - 1;
                if (channel.rotation_keys.size() <= current_frame_high)
                    current_frame_high = channel.rotation_keys.size() - 1;
                // 当前帧的下界不能超过上界
                current_frame_low = (current_frame_low < current_frame_high) ? current_frame_low : current_frame_high;

                // 将两帧动画进行插值, 应用到骨骼节点上
                Bone*      bone     = &m_bones[bone_index];
                Vector3    position = Vector3::lerp(channel.position_keys[current_frame_low], channel.position_keys[current_frame_high], lerp_ratio);
                Vector3    scaling  = Vector3::lerp(channel.scaling_keys[current_frame_low], channel.scaling_keys[current_frame_high], lerp_ratio);
                Quaternion rotation = Quaternion::nLerp(lerp_ratio, channel.rotation_keys[current_frame_low], channel.rotation_keys[current_frame_high], true);
                bone->rotate(rotation);
                bone->scale(scaling);
                bone->translate(position);
            }
        }

        /* 更新每个骨骼节点的 局部空间=>模型空间 变换 */
        for (size_t i = 0; i < m_bone_count; i++)
            m_bones[i].update();
    }
    /**
     * @brief 输出动画计算结果
     * @return 动画计算结果, 保存了每个节点的索引&变换矩阵
    */
    AnimationResult Skeleton::outputAnimationResult() {
        AnimationResult animation_result;

        for (size_t i = 0; i < m_bone_count; i++) {
            std::shared_ptr<AnimationResultElement> animation_result_element = std::make_shared<AnimationResultElement>();

            // 第i个骨骼节点
            Bone* bone = &m_bones[i];

            // 第i个骨骼节点的对应的索引
            animation_result_element->index = bone->getID() + 1;

            // 计算第i个骨骼节点的 局部空间=>模型空间 变换矩阵
            Vector3    temp_translation = bone->_getDerivedPosition();
            Quaternion temp_rotation    = bone->_getDerivedOrientation();
            Vector3    temp_scale       = bone->_getDerivedScale();
            auto       objMat           = Transform(temp_translation, temp_rotation, temp_scale).getMatrix();

            // Skinning Matrix = 骨骼节点到模型空间的变换 * Inverse Bind Pose Matrix
            auto resMat = objMat * bone->_getInverseTpose();

            // 转换为 Matrix4x4_ 格式
            animation_result_element->transform = resMat.toMatrix4x4_();

            // 将结果保存到 animation_result 中
            animation_result.node.push_back(*animation_result_element);
        }

        return animation_result;
    }

    void Skeleton::resetSkeleton() {
        for (size_t i = 0; i < m_bone_count; i++)
            m_bones[i].resetToInitialPose();
    }
    const Bone* Skeleton::getBones() const { return m_bones; }
    int32_t     Skeleton::getBonesCount() const { return m_bone_count; }
} // namespace MyPiccolo