#version 310 es

//-----------------------------------------------
// 快速近似抗锯齿: 仅在边界处进行抗锯齿处理
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* 输出数据 */
layout(location = 0) out vec2 out_uv; // 当前顶点对应屏幕中的坐标

void main() {
    const vec3 fullscreen_triangle_positions[3] = vec3[3](
        vec3(3.0, 1.0, 0.5),
        vec3(-1.0, 1.0, 0.5),
        vec3(-1.0, -3.0, 0.5));

    // 当前顶点对应屏幕中的坐标
    out_uv = 0.5 * (fullscreen_triangle_positions[gl_VertexIndex].xy + vec2(1.0, 1.0));

    gl_Position = vec4(fullscreen_triangle_positions[gl_VertexIndex], 1.0);
}