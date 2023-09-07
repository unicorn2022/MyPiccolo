#version 310 es

//--------------------------------------
// 点光影阴影: 光源位置+半径 => 阴影
// 1. 生成阴影贴图
//--------------------------------------

#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_geometry_shader : enable

#include "constants.vert"

/* uniform 数据 */
layout(set = 0, binding = 0) readonly buffer _unused_name_global_set_per_frame_binding_buffer {
    uint point_light_count;            // 点光源数量
    uint _padding_point_light_count_0; /* 填充为4个float大小 */
    uint _padding_point_light_count_1; /* 填充为4个float大小 */
    uint _padding_point_light_count_2; /* 填充为4个float大小 */

    highp vec4 point_lights_position_and_radius[m_max_point_light_count];
};

/* 输入数据 */
layout(location = 0) in highp float in_inv_length;                    // 顶点到光源的距离的倒数
layout(location = 1) in highp vec3 in_inv_length_position_view_space; // 顶点在光源空间的坐标*距离的倒数

/* 输出数据 */
layout(location = 0) out highp float out_depth;

/* 主函数 */
void main() {
    // 顶点在光源空间的位置
    highp vec3 position_view_space = in_inv_length_position_view_space * in_inv_length;

    // 点光源半径
    highp float point_light_radius = point_lights_position_and_radius[gl_Layer / 2].w;

    // 归一化深度值
    highp float ratio = length(position_view_space) / point_light_radius;

    // Trick: we don't write to depth, and thus we can use early depth test
    gl_FragDepth = ratio;
    out_depth    = ratio;
}