#version 310 es

//-----------------------------------------------
// 后处理
// 1. 定义一个屏幕大小的三角形, 覆盖整个屏幕
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

void main() {
    // 定义一个屏幕大小的三角形
    const vec3 fullscreen_triangle_positions[3] = vec3[3](
        vec3(3.0, 1.0, 0.5),
        vec3(-1.0, 1.0, 0.5),
        vec3(-1.0, -3.0, 0.5));

    gl_Position = vec4(fullscreen_triangle_positions[gl_VertexIndex], 1.0);
}