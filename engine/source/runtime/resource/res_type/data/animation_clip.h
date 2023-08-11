#pragma once

#include "runtime/core/math/transform.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <string>
#include <vector>

namespace MyPiccolo {
    REFLECTION_TYPE(AnimNodeMap)
    CLASS(AnimNodeMap, Fields)
    {
        REFLECTION_BODY(AnimNodeMap);

    public:
        std::vector<std::string> convert;
    };

    REFLECTION_TYPE(AnimationChannel)
    CLASS(AnimationChannel, Fields)
    {
        REFLECTION_BODY(AnimationChannel);

    public:
        std::string             name;          // 动画通道名称
        std::vector<Vector3>    position_keys; // 位置关键帧
        std::vector<Quaternion> rotation_keys; // 旋转关键帧
        std::vector<Vector3>    scaling_keys;  // 缩放关键帧
    };

    REFLECTION_TYPE(AnimationClip)
    CLASS(AnimationClip, Fields)
    {
        REFLECTION_BODY(AnimationClip);

    public:
        int                           total_frame {0}; // 总帧数
        int                           node_count {0};  // 动画通道数量
        std::vector<AnimationChannel> node_channels;   // 动画通道
    };

    REFLECTION_TYPE(AnimationAsset)
    CLASS(AnimationAsset, Fields)
    {
        REFLECTION_BODY(AnimationAsset);

    public:
        AnimNodeMap   node_map;          // 动画节点映射
        AnimationClip clip_data;         // 动画剪辑数据
        std::string   skelton_file_path; // 骨骼文件路径
    };
} // namespace MyPiccolo