#version 310 es

//-----------------------------------------------
// 延迟光照
// 1. 定义一个屏幕大小的三角形, 覆盖整个屏幕
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* 输出数据 */
layout(location = 0) out vec2 out_texcoord;

/* 主函数 */
void main() {
    // 定义一个屏幕大小的三角形
    vec3 fullscreen_triangle_positions[3] = vec3[3](
        vec3(3.0, 1.0, 0.5),
        vec3(-1.0, 1.0, 0.5),
        vec3(-1.0, -3.0, 0.5));

    // 三角形对应的纹理坐标
    vec2 fullscreen_triangle_uvs[3] = vec2[3](
        vec2(2.0, 1.0),
        vec2(0.0, 1.0),
        vec2(0.0, -1.0));

    gl_Position  = vec4(fullscreen_triangle_positions[gl_VertexIndex], 1.0);
    out_texcoord = fullscreen_triangle_uvs[gl_VertexIndex];
}