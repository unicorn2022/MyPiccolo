#version 310 es

//--------------------
//   蒙皮网格体绘制
//--------------------

#extension GL_GOOGLE_include_directive : enable

#include "../include/constants.h"

/* 定向光源 */
struct DirectionalLight {
    highp vec3 direction;          // 方向
    lowp float _padding_direction; // 方向对齐

    highp vec3 color;          // 颜色
    lowp float _padding_color; // 颜色对齐
};

/* 点光源 */
struct PointLight {
    highp vec3  position;           // 位置
    highp float radius;             // 半径
    highp vec3  intensity;          // 强度
    lowp float  _padding_intensity; // 强度对齐
};

/* uniform 数据 */
layout(set = 0, binding = 0) readonly buffer _unused_name_preframe {
    highp mat4 proj_view_matrix;         // 投影视图矩阵
    highp vec3 camera_position;          // 摄像机位置
    lowp float _padding_camera_position; // 摄像机位置对齐

    highp vec3 ambient_light;          // 环境光
    lowp float _padding_ambient_light; // 环境光对齐

    uint point_light_count;          // 点光源数量
    uint _padding_point_light_num_1; // 点光源数量对齐1
    uint _padding_point_light_num_2; // 点光源数量对齐2
    uint _padding_point_light_num_3; // 点光源数量对齐3

    PointLight       scene_point_lights[m_max_point_light_count]; // 点光源
    DirectionalLight scene_directional_light;                     // 定向光源
    highp mat4       directional_light_proj_view;                 // 定向光源proj_view矩阵
};
layout(set = 0, binding = 3) uniform sampler2D brdfLUT_sampler;                // brdf LUT
layout(set = 0, binding = 4) uniform samplerCube irradiance_sampler;           // 辐照度
layout(set = 0, binding = 5) uniform samplerCube specular_sampler;             // 镜面反射率
layout(set = 0, binding = 6) uniform highp sampler2DArray point_lights_shadow; // 点光源阴影
layout(set = 0, binding = 7) uniform highp sampler2D directional_light_shadow; // 定向光源阴影
