#version 310 es

//----------------------------------------------------------
// 天空盒绘制:
// 1. 计算当前顶点在裁剪空间的坐标
// 2. 输出当前顶点的UVW坐标
//----------------------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* 定向光源 */
struct DirectionalLight {
    vec3  direction;          // 方向
    float _padding_direction; /* 填充为4个float大小 */

    vec3  color;          // 颜色
    float _padding_color; /* 填充为4个float大小 */
};

/* 点光源 */
struct PointLight {
    vec3  position; // 位置
    float radius;   // 半径

    vec3  intensity;          // 强度
    float _padding_intensity; /* 填充为4个float大小 */
};

/* uniform 数据 */
layout(set = 0, binding = 0) readonly buffer _unused_name_preframe {
    mat4 proj_view_matrix; // 投影视图矩阵

    vec3  camera_position;          // 摄像机位置
    float _padding_camera_position; /* 填充为4个float大小 */

    vec3  ambient_light;          // 环境光
    float _padding_ambient_light; /* 填充为4个float大小 */

    uint point_light_num;            // 点光源数量
    uint _padding_point_light_num_1; /* 填充为4个float大小 */
    uint _padding_point_light_num_2; /* 填充为4个float大小 */
    uint _padding_point_light_num_3; /* 填充为4个float大小 */

    PointLight       scene_point_lights[m_max_point_light_count]; // 点光源
    DirectionalLight scene_directional_light;                     // 定向光源
    highp mat4       directional_light_proj_view;                 // 定向光源proj_view矩阵
};

/* 输出数据 */
layout(location = 0) out vec3 out_UVW;

/* 主函数 */
void main() {
    // 天空盒八个顶点的偏移量
    const vec3 cube_corner_vertex_offsets[8] = vec3[8](
        vec3(1.0, 1.0, 1.0),
        vec3(1.0, 1.0, -1.0),
        vec3(1.0, -1.0, -1.0),
        vec3(1.0, -1.0, 1.0),
        vec3(-1.0, 1.0, 1.0),
        vec3(-1.0, 1.0, -1.0),
        vec3(-1.0, -1.0, -1.0),
        vec3(-1.0, -1.0, 1.0));

    // 天空盒6个矩形面36个顶点的索引: x+, y+, x-, y-, z+, z-
    const int cube_triangle_index[36] = int[36](0, 1, 2, 2, 3, 0, 4, 5, 1, 1, 0, 4, 7, 6, 5, 5, 4, 7, 3, 2, 6, 6, 7, 3, 4, 0, 3, 3, 7, 4, 1, 5, 6, 6, 2, 1);

    // 世界坐标 = 摄像机位置 + 天空盒顶点偏移量
    vec3 world_position = camera_position + cube_corner_vertex_offsets[cube_triangle_index[gl_VertexIndex]];

    // 世界坐标 => NDC坐标
    vec4 clip_position = proj_view_matrix * vec4(world_position, 1.0);

    // 深度设置为0.99999
    clip_position.z = clip_position.w * 0.99999;

    // 当前顶点在裁剪空间的坐标
    gl_Position = clip_position;

    // 当前顶点的UVW坐标
    out_UVW = normalize(world_position - camera_position);
}