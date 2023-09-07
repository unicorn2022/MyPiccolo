#version 310 es

//--------------------------------------
// 点光影阴影: 光源位置+半径 => 阴影
// 1. 将顶点变换为正反两个面的光空间中的点
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

/* 几何着色器格式 */
layout(triangles) in;
layout(triangles_strip, max_vertices = m_max_point_light_geom_vertices) out;

/* 输入数据 */
layout(location = 0) in highp vec3 in_position_world_space[]; // 顶点在世界空间中的位置

/* 输出数据 */
layout(location = 0) out highp float out_inv_length;                    // 顶点到光源的距离的倒数
layout(location = 1) out highp vec3 out_inv_length_position_view_space; // 顶点在光源空间的坐标*距离的倒数

/* 主函数 */
void main() {
    for (highp int point_light_index = 0; point_light_index < int(point_light_count) && point_light_index < m_max_point_light_count; ++point_light_index) {
        vec3  point_light_position = point_lights_position_and_radius[point_light_index].xyz;
        float point_light_radius   = point_lights_position_and_radius[point_light_index].w;

        // 划分正反面
        for (highp int layer_index = 0; layer_index < 2; ++layer_index) {

            // 分别处理三角形的三个顶点
            for (highp int vertex_index = 0; vertex_index < 3; ++vertex_index) {
                // 顶点在世界空间中的位置
                highp vec3 position_world_space = in_position_world_space[vertex_index];

                // 顶点在光源空间中的位置
                highp vec3 position_view_space = position_world_space - point_light_position;
                // 归一化位置
                highp vec3 position_spherical_function_domain = normalize(position_view_space);

                // 划分正反两个光源空间对应的z坐标, 便于后续使用
                highp float layer_position_spherical_function_domain_z[2];
                layer_position_spherical_function_domain_z[0] = -position_spherical_function_domain.z;
                layer_position_spherical_function_domain_z[1] = position_spherical_function_domain.z;

                // 计算顶点在当前光源空间中的标准化坐标
                highp vec4 position_clip;
                position_clip.xy = position_spherical_function_domain.xy;
                position_clip.w  = layer_position_spherical_function_domain_z[layer_index] + 1.0;
                position_clip.z  = 0.5 * position_clip.w;
                gl_Position      = position_clip;

                out_inv_length                     = 1.0 / length(position_view_space);
                out_inv_length_position_view_space = out_inv_length * position_view_space;

                // 通过gl_Layer控制顶点将发送到哪一个纹理上
                gl_Layer = layer_index + 2 * point_light_index;
                EmitVertex();
            }
            EndPrimitive();
        }
    }
}