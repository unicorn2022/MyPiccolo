#pragma once

#include "runtime/resource/res_type/components/animation.h"

#include "runtime/function/animation/node.h"

namespace MyPiccolo {
    class SkeletonData;
    class BlendStateWithClipData;

    /* 角色骨骼树 */
    class Skeleton {
    private:
        bool  m_is_flat {false}; // 是否为扁平数组
        int   m_bone_count {0};  // 骨骼数量
        Bone* m_bones {nullptr}; // 骨骼数组

    public:
        ~Skeleton();

        /**
         * @brief 通过骨骼数据, 构建骨骼树
         * @param skeleton_defination 骨骼数据
        */
        void buildSkeleton(const SkeletonData& skeleton_defination);
        /**
         * @brief 通过混合状态数据, 将动画数据应用到骨骼树上
         * @param blend_state 混合状态数据, 只计算第一个clip动画
         * TODO: 优化, 可以同时计算多个clip动画
        */
        void applyAnimation(const BlendStateWithClipData& blend_state);
        /**
         * @brief 输出动画计算结果
         * @return 动画计算结果, 保存了每个节点的索引&变换矩阵
        */
        AnimationResult outputAnimationResult();

        void        resetSkeleton();
        const Bone* getBones() const;
        int32_t     getBonesCount() const;
    };
} // namespace MyPiccolo