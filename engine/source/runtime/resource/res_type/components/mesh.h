#pragma once

#include "runtime/core/math/transform.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace Piccolo {
    REFLECTION_TYPE(SubMeshRes)
    CLASS(SubMeshRes, Fields)
    {
        REFLECTION_BODY(SubMeshRes)

    public:
        std::string m_obj_file_ref; // obj文件引用
        Transform   m_transform;    // 变换
        std::string m_material;     // 材质
    };

    REFLECTION_TYPE(MeshComponentRes)
    CLASS(MeshComponentRes, Fields)
    {
        REFLECTION_BODY(MeshComponentRes)

    public:
        std::vector<SubMeshRes> m_sub_meshes; // 子网格
    };
} // namespace Piccolo