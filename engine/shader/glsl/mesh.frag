#version 310 es

//--------------------
//   蒙皮网格体绘制
//--------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"

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

    uint point_light_num;            // 点光源数量
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

layout(set = 2, binding = 0) uniform _unused_name_permaterial {
    highp vec4  baseColorFactor;   // 基础颜色因子
    highp float metallicFactor;    // 金属度因子
    highp float roughnessFactor;   // 粗糙度因子
    highp float normalScale;       // 法线缩放
    highp float occlusionStrength; // 遮挡强度
    highp vec3  emissiveFactor;    // 自发光因子
    uint        is_blend;          // 是否混合
    uint        is_double_sided;   // 是否双面
};
layout(set = 2, binding = 1) uniform sampler2D base_color_texture_sampler;         // 基础颜色纹理
layout(set = 2, binding = 2) uniform sampler2D metallic_roughness_texture_sampler; // 金属度粗糙度纹理
layout(set = 2, binding = 3) uniform sampler2D normal_texture_sampler;             // 法线纹理
layout(set = 2, binding = 4) uniform sampler2D occlusion_texture_sampler;          // 遮挡纹理
layout(set = 2, binding = 5) uniform sampler2D emissive_color_texture_sampler;     // 自发光纹理

/* 输入数据 */
layout(location = 0) in highp vec3 in_world_position; // 世界坐标
layout(location = 1) in highp vec3 in_normal;         // 法线
layout(location = 2) in highp vec3 in_tagent;         // 切线
layout(location = 3) in highp vec2 in_texcoord;       // 纹理坐标

/* 输出数据 */
layout(location = 0) out highp vec4 out_scene_color; // 颜色

// 获取基础颜色
highp vec3 getBaseColor() {
    highp vec3 basecolor = texture(base_color_texture_sampler, in_texcoord).xyz * baseColorFactor.xyz;
    return basecolor;
}

// 计算切线空间下的单位法线
highp vec3 calculateNormal() {
    // 采样法线向量: [0,1] => [-1,1]
    highp vec3 tangent_normal = texture(normal_texture_sampler, in_texcoord).xyz * 2.0 - 1.0;

    highp vec3 N = normalize(in_normal);     // 法线  (z)
    highp vec3 T = normalize(in_tagent.xyz); // 切线  (x, 对应U轴)
    highp vec3 B = normalize(cross(N, T));   // 副法线 (y, 对应V轴)

    // 切线空间坐标系
    highp TBN = mat3(T, B, N);

    // 法线贴图的法线向量转换到切线空间下
    return normalize(TBN * tangent_normal);
}

#include "mesh_lighting_H.frag"

/* 主函数 */
void main() {
    highp vec3  N                   = calculateNormal();
    highp vec3  basecolor           = getBaseColor();
    highp float metallic            = texture(metallic_roughness_texture_sampler, in_texcoord).z * metallicFactor;
    highp float dielectric_specular = 0.04; // 电介质镜面
    highp float roughness           = texture(metallic_roughness_texture_sampler, in_texcoord).y * roughnessFactor;

    highp vec3 result_color;

#include "mesh_lighting_INL.frag"

    out_scene_color = vec4(result_color, 1.0);
}