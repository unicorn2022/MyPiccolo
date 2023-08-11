#pragma once

#include "runtime/resource/res_type/data/basic_shape.h"

#include "runtime/core/math/axis_aligned.h"
#include "runtime/core/math/transform.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace MyPiccolo {
    /* 刚体形状 */
    enum class RigidBodyShapeType : unsigned char {
        box,     // 立方体
        sphere,  // 球体
        capsule, // 胶囊体
        invalid
    };

    /* 刚体形状 */
    REFLECTION_TYPE(RigidBodyShape)
    CLASS(RigidBodyShape, WhiteListFields)
    {
        REFLECTION_BODY(RigidBodyShape);

    public:
        Transform          m_global_transform;                   // 全局变换
        AxisAlignedBox     m_bounding_box;                       // 轴对齐包围盒
        RigidBodyShapeType m_type {RigidBodyShapeType::invalid}; // 类型

        META(Enable)
        Transform m_local_transform; // 局部变换
        META(Enable)
        Reflection::ReflectionPtr<Geometry> m_geometry; // 几何体

        RigidBodyShape() = default;
        RigidBodyShape(const RigidBodyShape& res);

        ~RigidBodyShape();
    };

    /* 刚体组件数据 */
    REFLECTION_TYPE(RigidBodyComponentRes)
    CLASS(RigidBodyComponentRes, Fields)
    {
        REFLECTION_BODY(RigidBodyComponentRes);

    public:
        std::vector<RigidBodyShape> m_shapes;       // 形状
        float                       m_inverse_mass; // 质量倒数
        int                         m_actor_type;   // 角色类型
    };

} // namespace MyPiccolo