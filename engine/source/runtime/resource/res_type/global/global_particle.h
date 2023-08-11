#pragma once

#include "runtime/core/math/vector3.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace MyPiccolo {

    REFLECTION_TYPE(GlobalParticleRes)
    CLASS(GlobalParticleRes, Fields)
    {
        REFLECTION_BODY(GlobalParticleRes);

    public:
        GlobalParticleRes() {}

        META(Enable)
        int         m_emit_gap;                        // 每隔多少帧发射一次
        int         m_emit_count;                      // 每次发射多少个粒子
        float       m_time_step;                       // 时间步长
        float       m_max_life;                        // 最大生命值
        Vector3     m_gravity;                         // 重力
        std::string m_particle_billboard_texture_path; // 粒子贴图路径
        std::string m_piccolo_logo_texture_path;       // Piccolo Logo贴图路径
    };
} // namespace MyPiccolo