#pragma once

#include "runtime/core/meta/reflection/reflection.h"

#include "runtime/core/math/vector2.h"
#include "runtime/core/math/vector3.h"

namespace Piccolo {
    REFLECTION_TYPE(CameraPose)
    CLASS(CameraPose, Fields)
    {
        REFLECTION_BODY(CameraPose);

    public:
        Vector3 m_position; // 相机位置
        Vector3 m_target;   // 相机朝向
        Vector3 m_up;       // 相机上方向
    };

    REFLECTION_TYPE(CameraConfig)
    CLASS(CameraConfig, Fields)
    {
        REFLECTION_BODY(CameraConfig);

    public:
        CameraPose m_pose;   // 相机姿态
        Vector2    m_aspect; // 相机视口宽高比
        float      m_z_far;  // 相机远平面
        float      m_z_near; // 相机近平面
    };
} // namespace Piccolo