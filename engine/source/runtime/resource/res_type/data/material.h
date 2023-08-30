#pragma once

#include "runtime/core/meta/reflection/reflection.h"

namespace Piccolo {
    REFLECTION_TYPE(MaterialRes)
    CLASS(MaterialRes, Fields)
    {
        REFLECTION_BODY(MaterialRes);

    public:
        std::string m_base_colour_texture_file;        // 基础颜色贴图
        std::string m_metallic_roughness_texture_file; // 金属粗糙度贴图
        std::string m_normal_texture_file;             // 法线贴图
        std::string m_occlusion_texture_file;          // 遮挡贴图
        std::string m_emissive_texture_file;           // 自发光贴图
    };
} // namespace Piccolo