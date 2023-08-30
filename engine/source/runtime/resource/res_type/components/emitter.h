#pragma once

#include "runtime/core/math/matrix4.h"
#include "runtime/core/math/vector2.h"
#include "runtime/core/math/vector4.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace Piccolo {
    REFLECTION_TYPE(ParticleComponentRes)
    CLASS(ParticleComponentRes, Fields)
    {
        REFLECTION_BODY(ParticleComponentRes);

    public:
        Vector3    m_local_translation; // 局部位置
        Quaternion m_local_rotation;    // 局部旋转
        Vector4    m_velocity;          // 基础速度
        Vector4    m_acceleration;      // 基础加速度
        Vector3    m_size;              // 基础大小
        int        m_emitter_type;      // 发射器类型
        Vector2    m_life;              // 生命周期
        Vector4    m_color;             // 基础颜色
    };
} // namespace Piccolo