#pragma once

#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/resource/res_type/data/animation_clip.h"
#include "runtime/resource/res_type/data/animation_skeleton_node_map.h"

#include <string>
#include <vector>

namespace MyPiccolo {
    REFLECTION_TYPE(BoneBlendWeight)
    CLASS(BoneBlendWeight, Fields)
    {
        REFLECTION_BODY(BoneBlendWeight);

    public:
        std::vector<float> blend_weight; // 混合权重
    };

    REFLECTION_TYPE(BlendStateWithClipData)
    CLASS(BlendStateWithClipData, Fields)
    {
        REFLECTION_BODY(BlendStateWithClipData);

    public:
        int                          clip_count;          // 混合动画剪辑数量
        std::vector<AnimationClip>   blend_clip;          // 混合动画剪辑
        std::vector<AnimSkelMap>     blend_anim_skel_map; // 混合动画骨骼节点映射
        std::vector<BoneBlendWeight> blend_weight;        // 混合权重
        std::vector<float>           blend_ratio;         // 混合比例
    };

    REFLECTION_TYPE(BlendState)
    CLASS(BlendState, Fields)
    {
        REFLECTION_BODY(BlendState);

    public:
        int                      clip_count;               // 混合动画剪辑数量
        std::vector<std::string> blend_clip_file_path;     // 混合动画剪辑文件路径
        std::vector<float>       blend_clip_file_length;   // 混合动画剪辑文件长度
        std::vector<std::string> blend_anim_skel_map_path; // 混合动画骨骼节点映射文件路径
        std::vector<float>       blend_weight;             // 混合权重
        std::vector<std::string> blend_mask_file_path;     // 混合动画遮罩文件路径
        std::vector<float>       blend_ratio;              // 混合比例
    };
} // namespace MyPiccolo