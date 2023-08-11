#pragma once

#include "runtime/core/math/quaternion.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace MyPiccolo {
    REFLECTION_TYPE(CameraParameter)
    CLASS(CameraParameter, Fields)
    {
        REFLECTION_BODY(CameraParameter);

    public:
        virtual ~CameraParameter() {}

        float m_fov {50.f}; // 视野
    };

    /* 第一人称相机参数 */
    REFLECTION_TYPE(FirstPersonCameraParameter)
    CLASS(FirstPersonCameraParameter
          : public CameraParameter, Fields)
    {
        REFLECTION_BODY(FirstPersonCameraParameter);

    public:
        float m_vertical_offset {0.6f}; // 垂直偏移
    };

    /* 第三人称相机参数 */
    REFLECTION_TYPE(ThirdPersonCameraParameter)
    CLASS(ThirdPersonCameraParameter
          : public CameraParameter, WhiteListFields)
    {
        REFLECTION_BODY(ThirdPersonCameraParameter);

    public:
        META(Enable)
        float m_horizontal_offset {3.f}; // 水平偏移
        META(Enable)
        float m_vertical_offset {2.5f}; // 垂直偏移

        Quaternion m_cursor_pitch; // 光标俯仰
        Quaternion m_cursor_yaw;   // 光标偏航
    };

    /* 自由相机参数 */
    REFLECTION_TYPE(FreeCameraParameter)
    CLASS(FreeCameraParameter
          : public CameraParameter, Fields)
    {
        REFLECTION_BODY(FreeCameraParameter);

    public:
        float m_speed {1.f}; // 速度
    };

    REFLECTION_TYPE(CameraComponentRes)
    CLASS(CameraComponentRes, Fields)
    {
        REFLECTION_BODY(CameraComponentRes);

    public:
        Reflection::ReflectionPtr<CameraParameter> m_parameter; // 相机参数

        CameraComponentRes() = default;
        CameraComponentRes(const CameraComponentRes& res);

        ~CameraComponentRes();
    };
} // namespace MyPiccolo