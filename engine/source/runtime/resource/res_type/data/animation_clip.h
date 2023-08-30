#pragma once

#include "runtime/core/math/transform.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <string>
#include <vector>

namespace Piccolo {
    /* 动画节点映射 */
    REFLECTION_TYPE(AnimNodeMap)
    CLASS(AnimNodeMap, Fields)
    {
        REFLECTION_BODY(AnimNodeMap);

    public:
        std::vector<std::string> convert; // index => 骨骼节点名称
    };

    /* 动画通道数据 */
    REFLECTION_TYPE(AnimationChannel)
    CLASS(AnimationChannel, Fields)
    {
        REFLECTION_BODY(AnimationChannel);

    public:
        std::string             name;          // 动画通道名称
        std::vector<Vector3>    position_keys; // 关键帧: 位置
        std::vector<Quaternion> rotation_keys; // 关键帧: 旋转
        std::vector<Vector3>    scaling_keys;  // 关键帧: 缩放
    };

    /* clip 数据 */
    REFLECTION_TYPE(AnimationClip)
    CLASS(AnimationClip, Fields)
    {
        REFLECTION_BODY(AnimationClip);

    public:
        int                           total_frame {0}; // 总帧数
        int                           node_count {0};  // 动画通道数量
        std::vector<AnimationChannel> node_channels;   // 动画通道
    };

    /* 动画资产 */
    REFLECTION_TYPE(AnimationAsset)
    CLASS(AnimationAsset, Fields)
    {
        REFLECTION_BODY(AnimationAsset);

    public:
        AnimNodeMap   node_map;          // index => 骨骼节点名称
        AnimationClip clip_data;         // clip 数据
        std::string   skelton_file_path; // 骨骼文件路径
    };
} // namespace Piccolo