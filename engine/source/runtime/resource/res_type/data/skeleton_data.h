#pragma once

#include "runtime/core/math/transform.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <string>
#include <vector>

namespace MyPiccolo {
    /* 骨骼节点数据 */
    REFLECTION_TYPE(RawBone)
    CLASS(RawBone, Fields)
    {
        REFLECTION_BODY(RawBone);

    public:
        std::string name;         // 骨骼名称
        int         index;        // 骨骼索引
        Transform   binding_pose; // 绑定姿态
        Matrix4x4_  tpose_matrix; // 绑定姿态矩阵
        int         parent_index; // 父骨骼索引
    };

    /* 骨骼树数据 */
    REFLECTION_TYPE(SkeletonData)
    CLASS(SkeletonData, Fields)
    {
        REFLECTION_BODY(SkeletonData);

    public:
        std::vector<RawBone> bones_map;                    // index => 骨骼节点数据
        bool                 is_flat = false;              // bone.index和index是否在bones_map中相同
        int                  root_index;                   // 根骨骼索引
        bool                 in_topological_order = false; // 是否已经按照拓扑排序
    };
} // namespace MyPiccolo