#version 310 es

//-----------------------------------------------
// 后处理-绘制UI
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* uniform 数据 */
layout(input_attachment_index = 0, set = 0, binding = 0) uniform highp subpassInput in_scene_color; // 场景颜色
layout(input_attachment_index = 1, set = 0, binding = 1) uniform highp subpassInput in_ui_color;    // UI颜色

/* 输出数据 */
layout(location = 0) out highp vec4 out_color;

/* 主函数 */
void main() {
    highp vec4 scene_color = subpassLoad(in_scene_color).rgba;
    highp vec4 ui_color    = subpassLoad(in_ui_color).rgba;

    // Gamma矫正: color = color ^ (1.0 / γ)
    if (ui_color.r < 1e-6 && ui_color.g < 1e-6 && ui_color.a < 1e-6) {
        ui_color  = vec4(pow(ui_color.r, 1.0 / 2.2), pow(ui_color.g, 1.0 / 2.2), pow(ui_color.b, 1.0 / 2.2), pow(ui_color.a, 1.0 / 2.2));
        out_color = scene_color;
    } else {
        ui_color  = vec4(pow(ui_color.r, 1.0 / 2.2), pow(ui_color.g, 1.0 / 2.2), pow(ui_color.b, 1.0 / 2.2), pow(ui_color.a, 1.0 / 2.2));
        out_color = ui_color;
    }
}