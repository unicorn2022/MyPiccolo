#pragma once

#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/resource/res_type/data/blend_state.h"

#include <string>
#include <vector>

namespace MyPiccolo {
    REFLECTION_TYPE(AnimationResultElement)
    CLASS(AnimationResultElement, WhiteListFields)
    {
        REFLECTION_BODY(AnimationResultElement)

    public:
        int        index;     // 索引
        Matrix4x4_ transform; // 变换矩阵
    };

    REFLECTION_TYPE(AnimationResult)
    CLASS(AnimationResult, Fields)
    {
        REFLECTION_BODY(AnimationResult)

    public:
        std::vector<AnimationResultElement> node; // 动画节点
    };

    REFLECTION_TYPE(AnimationComponentRes)
    CLASS(AnimationComponentRes, Fields)
    {
        REFLECTION_BODY(AnimationComponentRes)

    public:
        std::string skeletion_file_path; // 骨骼文件路径
        BlendState  blend_state;         // 混合空间
        float       frame_position;      // 帧位置 0-1

        META(Disable)
        AnimationResult animation_result; // 动画结果
    };
} // namespace MyPiccolo