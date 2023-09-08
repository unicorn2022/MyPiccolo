#version 310 es

//-----------------------------------------------
// 后处理-tone mapping: 实现正确曝光
//  (1) 通过曝光曲线, 将HDR颜色映射到SDR颜色
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* uniform 数据 */
layout(input_attachment_index = 0, set = 0, binding = 0) uniform highp subpassInput in_color; // 当前颜色

/* 输出数据 */
layout(location = 0) out highp vec4 out_color;

// tonemap算法: 通过曝光曲线, 将HDR颜色映射到SDR颜色
highp vec3 Uncharted2Tonemap(highp vec3 x) {
    highp float A = 0.15;
    highp float B = 0.50;
    highp float C = 0.10;
    highp float D = 0.20;
    highp float E = 0.02;
    highp float F = 0.30;
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

/* 主函数 */
void main() {
    // 当前颜色
    highp vec3 color = subpassLoad(in_color).rgb;
    // tone mapping
    color = Uncharted2Tonemap(color * 4.5f);
    color = color * (1.0f / Uncharted2Tonemap(vec3(11.2f)));

    // Gamma矫正: color = color ^ (1.0 / γ)
    color = vec3(pow(color.x, 1.0 / 2.2), pow(color.y, 1.0 / 2.2), pow(color.z, 1.0 / 2.2));

    // 输出颜色
    out_color = vec4(color, 1.0f);
}