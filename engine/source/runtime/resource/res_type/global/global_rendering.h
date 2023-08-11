#pragma once

#include "runtime/core/meta/reflection/reflection.h"

#include "runtime/core/color/color.h"
#include "runtime/core/math/vector3.h"

#include "runtime/resource/res_type/data/camera_config.h"

namespace MyPiccolo {
    REFLECTION_TYPE(SkyBoxIrradianceMap)
    CLASS(SkyBoxIrradianceMap, Fields)
    {
        REFLECTION_BODY(SkyBoxIrradianceMap)

    public:
        std::string m_negative_x_map; // -x方向的贴图
        std::string m_positive_x_map; // +x方向的贴图
        std::string m_negative_y_map; // -y方向的贴图
        std::string m_positive_y_map; // +y方向的贴图
        std::string m_negative_z_map; // -z方向的贴图
        std::string m_positive_z_map; // +z方向的贴图
    };

    REFLECTION_TYPE(SkyBoxSpecularMap)
    CLASS(SkyBoxSpecularMap, Fields)
    {
        REFLECTION_BODY(SkyBoxSpecularMap)

    public:
        std::string m_negative_x_map; // -x方向的贴图
        std::string m_positive_x_map; // +x方向的贴图
        std::string m_negative_y_map; // -y方向的贴图
        std::string m_positive_y_map; // +y方向的贴图
        std::string m_negative_z_map; // -z方向的贴图
        std::string m_positive_z_map; // +z方向的贴图
    };

    REFLECTION_TYPE(DirectionalLight)
    CLASS(DirectionalLight, Fields)
    {
        REFLECTION_BODY(DirectionalLight)

    public:
        Vector3 m_direction; // 光照方向
        Color   m_color;     // 光照颜色
    };

    REFLECTION_TYPE(GlobalRenderingRes)
    CLASS(GlobalRenderingRes, Fields)
    {
        REFLECTION_BODY(GlobalRenderingRes);

    public:
        bool                m_enable_fxaa {false};   // 是否开启fxaa抗锯齿
        SkyBoxIrradianceMap m_skybox_irradiance_map; // 天空盒辐照度贴图
        SkyBoxSpecularMap   m_skybox_specular_map;   // 天空盒高光贴图
        std::string         m_brdf_map;              // BRDF贴图
        std::string         m_color_grading_map;     // color grading 贴图

        Color            m_sky_color;         // 天空颜色
        Color            m_ambient_light;     // 环境光颜色
        CameraConfig     m_camera_config;     // 相机配置
        DirectionalLight m_directional_light; // 定向光源
    };

} // namespace MyPiccolo