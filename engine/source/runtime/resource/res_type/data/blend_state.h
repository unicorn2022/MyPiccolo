#pragma once

#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/resource/res_type/data/animation_clip.h"
#include "runtime/resource/res_type/data/animation_skeleton_node_map.h"

#include <string>
#include <vector>

namespace MyPiccolo {
    /* 当前动画的混合权重 */
    REFLECTION_TYPE(BoneBlendWeight)
    CLASS(BoneBlendWeight, Fields)
    {
        REFLECTION_BODY(BoneBlendWeight);

    public:
        std::vector<float> blend_weight; // 每个骨骼节点的混合权重
    };

    /* 包含 clip 数据的 blend_state*/
    REFLECTION_TYPE(BlendStateWithClipData)
    CLASS(BlendStateWithClipData, Fields)
    {
        REFLECTION_BODY(BlendStateWithClipData);

    public:
        int                          clip_count;          // clip的数量
        std::vector<AnimationClip>   blend_clip;          // clip数据
        std::vector<AnimSkelMap>     blend_anim_skel_map; // index => 骨骼节点编号
        std::vector<BoneBlendWeight> blend_weight;        // clip的混合权重
        std::vector<float>           blend_ratio;         // clip已经播放的时间占比
    };

    /* 混合状态 */
    REFLECTION_TYPE(BlendState)
    CLASS(BlendState, Fields)
    {
        REFLECTION_BODY(BlendState);

    public:
        int                      clip_count;               // clip的数量
        std::vector<std::string> blend_clip_file_path;     // clip的文件路径
        std::vector<float>       blend_clip_file_length;   // clip的文件长度
        std::vector<std::string> blend_anim_skel_map_path; // 混合动画骨骼节点映射文件路径
        std::vector<float>       blend_weight;             // 混合权重
        std::vector<std::string> blend_mask_file_path;     // 混合动画遮罩文件路径
        std::vector<float>       blend_ratio;              // 混合比例
    };
} // namespace MyPiccolo