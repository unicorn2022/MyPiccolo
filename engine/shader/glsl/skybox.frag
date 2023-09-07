#version 310 es

//----------------------------------------------------------
// 天空盒绘制:
// 1. 采样当前方向的颜色, 并输出
//----------------------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"
#include "gbuffer.frag"

/* uniform数据 */
layout(set = 0, binding = 1) uniform samplerCube specular_sampler;

/* 输入数据 */
layout(location = 0) in highp vec3 in_UVW;

/* 输出数据 */
layout(location = 0) out highp vec4 out_scene_color;

/* 主函数 */
void main() {
    // UVW坐标转换
    highp vec3 origin_sample_UVW = vec3(in_UVW.x, in_UVW.z, in_UVW.y);

    // 采样天空盒
    highp vec3 color = textureLod(specular_sampler, origin_sample_UVW, 0.0).rgb;

    // 输出
    out_scene_color = vec4(color, 1.0);
}