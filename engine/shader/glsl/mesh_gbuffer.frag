#version 310 es

//-----------------------------------------------
// 将网格体绘制到GBuffer中
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"
#include "gbuffer.frag"

/* uniform 数据 */
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
layout(location = 0) out highp vec4 out_gbuffer_a; // GBuffer A
layout(location = 1) out highp vec4 out_gbuffer_b; // GBuffer B
layout(location = 2) out highp vec4 out_gbuffer_c; // GBuffer C
// layout(location = 3) out highp vec4 out_scene_color; // 场景颜色

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

/* 主函数 */
void main() {
    // 生成当前像素的GBuffer数据
    PGBufferData gbuffer;
    gbuffer.worldNormal    = calculateNormal();
    gbuffer.baseColor      = getBasecolor();
    gbuffer.metallic       = texture(metallic_roughness_texture_sampler, in_texcoord).z * metallicFactor;
    gbuffer.specular       = 0.5;
    gbuffer.roughness      = texture(metallic_roughness_texture_sampler, in_texcoord).y * roughnessFactor;
    gbuffer.shadingModelID = SHADINGMODELID_DEFAULT_LIT;

    // 自发光
    highp vec3 Le = texture(emissive_color_texture_sampler, in_texcoord).xyz * emissiveFactor;
    // out_scene_color.rgba = vec4(Le, 1.0);

    // 编码当前像素的GBuffer数据
    EncodeGBufferData(gbuffer, out_gbuffer_a, out_gbuffer_b, out_gbuffer_c);
}