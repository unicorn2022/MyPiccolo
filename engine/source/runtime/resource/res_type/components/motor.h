#pragma once

#include "runtime/core/math/vector3.h"
#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/resource/res_type/data/basic_shape.h"

namespace Piccolo {
    /* 控制器类型 */
    enum class ControllerType : unsigned char {
        none,    // 无
        physics, // 物理
        invalid
    };

    /* 控制器配置 */
    REFLECTION_TYPE(ControllerConfig)
    CLASS(ControllerConfig, Fields)
    {
        REFLECTION_BODY(ControllerConfig);

    public:
        virtual ~ControllerConfig() {}
    };

    /* 物理控制器配置 */
    REFLECTION_TYPE(PhysicsControllerConfig)
    CLASS(PhysicsControllerConfig
          : public ControllerConfig, Fields)
    {
        REFLECTION_BODY(PhysicsControllerConfig);

    public:
        PhysicsControllerConfig() {}
        ~PhysicsControllerConfig() {}
        Capsule m_capsule_shape; // 胶囊体形状
    };

    /* 运动组件数据 */
    REFLECTION_TYPE(MotorComponentRes)
    CLASS(MotorComponentRes, Fields)
    {
        REFLECTION_BODY(MotorComponentRes);

    public:
        MotorComponentRes() = default;
        ~MotorComponentRes() {}

        float m_move_speed {0.f};             // 移动速度
        float m_jump_height {0.f};            // 跳跃高度
        float m_max_move_speed_ratio {0.f};   // 最大移动速度比例
        float m_max_sprint_speed_ratio {0.f}; // 最大冲刺速度比例
        float m_move_acceleration {0.f};      // 移动加速度
        float m_sprint_acceleration {0.f};    // 冲刺加速度

        Reflection::ReflectionPtr<ControllerConfig> m_controller_config; // 控制器配置
    };
} // namespace Piccolo