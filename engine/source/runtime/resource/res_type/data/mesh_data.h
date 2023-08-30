#pragma once

#include "runtime/core/meta/reflection/reflection.h"

#include <string>
#include <vector>

namespace Piccolo {
    /* 顶点数据 */
    REFLECTION_TYPE(Vertex)
    CLASS(Vertex, Fields)
    {
        REFLECTION_BODY(Vertex);

    public:
        float px, py, pz; // 位置
        float nx, ny, nz; // 法线
        float tx, ty, tz; // 切线
        float u, v;       // 纹理坐标
    };

    /* 顶点绑定骨骼节点 */
    REFLECTION_TYPE(SkeletonBinding)
    CLASS(SkeletonBinding, Fields)
    {
        REFLECTION_BODY(SkeletonBinding);

    public:
        int   index0, index1, index2, index3;     // 骨骼索引
        float weight0, weight1, weight2, weight3; // 骨骼权重
    };

    /* 网格体数据 */
    REFLECTION_TYPE(MeshData)
    CLASS(MeshData, Fields)
    {
        REFLECTION_BODY(MeshData);

    public:
        std::vector<Vertex>          vertex_buffer; // 顶点缓冲
        std::vector<int>             index_buffer;  // 索引缓冲
        std::vector<SkeletonBinding> bind;          // 骨骼绑定
    };
} // namespace Piccolo