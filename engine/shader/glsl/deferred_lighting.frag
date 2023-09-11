#version 310 es

//-----------------------------------------------
// 延迟光照
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"
#include "gbuffer.frag"

/* 定向光源 */
struct DirectionalLight {
    highp vec3 direction;          // 方向
    lowp float _padding_direction; /* 填充为4个float大小 */

    highp vec3 color;          // 颜色
    lowp float _padding_color; /* 填充为4个float大小 */
};

/* 点光源 */
struct PointLight {
    highp vec3  position; // 位置
    highp float radius;   // 半径

    highp vec3 intensity;          // 强度
    lowp float _padding_intensity; /* 填充为4个float大小 */
};

/* uniform 数据 */
layout(set = 0, binding = 0) readonly buffer _unused_name_preframe {
    highp mat4 proj_view_matrix; // 投影视图矩阵

    highp vec3 camera_position;          // 摄像机位置
    lowp float _padding_camera_position; /* 填充为4个float大小 */

    highp vec3 ambient_light;          // 环境光
    lowp float _padding_ambient_light; /* 填充为4个float大小 */

    uint point_light_num;            // 点光源数量
    uint _padding_point_light_num_1; /* 填充为4个float大小 */
    uint _padding_point_light_num_2; /* 填充为4个float大小 */
    uint _padding_point_light_num_3; /* 填充为4个float大小 */

    PointLight       scene_point_lights[m_max_point_light_count]; // 点光源
    DirectionalLight scene_directional_light;                     // 定向光源
    highp mat4       directional_light_proj_view;                 // 定向光源proj_view矩阵
};
layout(set = 0, binding = 3) uniform sampler2D brdfLUT_sampler;                // brdf LUT
layout(set = 0, binding = 4) uniform samplerCube irradiance_sampler;           // 环境贴图: 辐照度
layout(set = 0, binding = 5) uniform samplerCube specular_sampler;             // 环境贴图: 镜面反射率
layout(set = 0, binding = 6) uniform highp sampler2DArray point_lights_shadow; // 点光源阴影(每个点光源占据2个2D贴图)
layout(set = 0, binding = 7) uniform highp sampler2D directional_light_shadow; // 定向光源阴影

layout(input_attachment_index = 0, set = 1, binding = 0) uniform highp subpassInput in_gbuffer_a;   // GBuffer A
layout(input_attachment_index = 0, set = 1, binding = 1) uniform highp subpassInput in_gbuffer_b;   // GBuffer B
layout(input_attachment_index = 0, set = 1, binding = 2) uniform highp subpassInput in_gbuffer_c;   // GBuffer C
layout(input_attachment_index = 0, set = 1, binding = 3) uniform highp subpassInput in_scene_depth; // 场景深度

layout(set = 2, binding = 1) uniform samplerCube skybox_sampler; // 天空盒

/* 输入数据 */
layout(location = 0) in highp vec2 in_texcoord; // 纹理坐标

/* 输出数据 */
layout(location = 0) out highp vec4 out_color;

#include "mesh_lighting_H.frag"

/* 主函数 */
void main() {
    highp vec4 gbuffer_a = subpassLoad(in_gbuffer_a).rgba;
    highp vec4 gbuffer_b = subpassLoad(in_gbuffer_b).rgba;
    highp vec4 gbuffer_c = subpassLoad(in_gbuffer_c).rgba;

    // 解码GBuffer数据
    PGBufferData gbuffer;
    DecodeGBufferData(gbuffer, gbuffer_a, gbuffer_b, gbuffer_c);
    // 获取法线、颜色、金属都、高光度、粗糙度信息
    highp vec3  N                   = gbuffer.worldNormal;
    highp vec3  basecolor           = gbuffer.baseColor;
    highp float metallic            = gbuffer.metallic;
    highp float dielectric_specular = 0.08 * gbuffer.specular;
    highp float roughness           = gbuffer.roughness;

    // 获取当前像素对应的场景坐标
    highp vec3 in_world_position;
    {
        // 当前像素的深度
        highp float scene_depth = subpassLoad(in_scene_depth).r;
        // 当前像素的对应的ndc标准坐标
        highp vec4 ndc = vec4(uv_to_ndcxy(in_texcoord), scene_depth, 1.0);
        // 逆变换到世界坐标
        highp mat4 inverse_proj_view_matrix = inverse(proj_view_matrix);
        highp vec4 in_world_position_with_w = inverse_proj_view_matrix * ndc;
        // 标准化坐标
        in_world_position = in_world_position_with_w.xyz / in_world_position_with_w.www;
    }

    // 计算光照结果
    highp vec3 result_color = vec3(0.0, 0.0, 0.0);

    // 着色模型: 无光照
    if (SHADINGMODELID_UNLIT == gbuffer.shadingModelID) {
        // 计算从相机位置指向片段位置的方向向量
        highp vec3 in_UVW = normalize(in_world_position - camera_position);
        // 交换Y和Z坐标以适应天空盒纹理坐标: 通常天空盒的纹理坐标布局可能会将y轴与纹理的高度对应
        highp vec3 origin_sample_UVW = vec3(in_UVW.x, in_UVW.z, in_UVW.y);
        // 对天空盒进行采样
        result_color = textureLod(skybox_sampler, origin_sample_UVW, 0.0).rgb;
    }
    // 着色模型: 默认光照
    else if (SHADINGMODELID_DEFAULT_LIT == gbuffer.shadingModelID) {
#include "mesh_lighting.inl"
    }

    out_color = vec4(result_color, 1.0f);
}