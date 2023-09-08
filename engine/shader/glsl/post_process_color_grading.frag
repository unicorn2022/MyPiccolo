#version 310 es

//-----------------------------------------------
// 后处理-color_grading: 实现风格化表达
//  (1) 通过LUT查找表, 将原始颜色转换为风格化颜色
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* uniform 数据 */
layout(input_attachment_index = 0, set = 0, binding = 0) uniform highp subpassInput in_color; // 当前颜色
layout(set = 0, binding = 1) uniform sampler2D color_grading_lut_texture_sampler;             // color grading LUT查找表

/* 输出数据 */
layout(location = 0) out highp vec4 out_color;

/* 主函数 */
void main() {
    // LUT查找表的大小
    highp ivec2 lut_tex_size = textureSize(color_grading_lut_texture_sampler, 0);
    // 当前颜色
    highp vec4 color = subpassLoad(in_color).rgba;

    // TODO: 请在此处实现color grading

    // 输出颜色
    out_color = color;
}