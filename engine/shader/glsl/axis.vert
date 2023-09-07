#version 460 core
//--------------------
//     坐标轴绘制
//--------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* 点光源 */
struct PointLight {
    vec3  position;           // 位置
    float radius;             // 半径
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

    PointLight scene_point_lights[m_max_point_light_count]; // 点光源
};
layout(set = 0, binding = 1) readonly buffer _unusedname_axis {
    mat4 model_matrix;  // 模型矩阵
    uint selected_axis; // 选中的坐标轴
};

/* 输入数据 */
layout(location = 0) in vec3 in_position; // 位置
layout(location = 1) in vec3 in_normal;   // 法线
layout(location = 2) in vec3 in_tangent;  // 切线
layout(location = 3) in vec2 in_texcoord; // 纹理坐标

/* 输出数据 */
layout(location = 0) out vec3 out_color;

void main() {
    // 计算投影后的位置
    vec3 world_position = (model_matrix * vec4(in_position, 1.0)).xyz;
    vec4 clip_position  = proj_view_matrix * vec4(world_position, 1.0);

    // 将深度设置为0.0001, 使得坐标轴绘制在其他物体之前
    clip_position.z = clip_position.z * 0.0001;
    gl_Position     = clip_position;

    if (in_texcoord.x < 0.01f) {
        if (selected_axis == 0) {
            out_color = vec3(1.0, 1.0, 0.0);
        } else {
            out_color = vec3(1.0, 0.0, 0.0);
        }
    } else if (in_texcoord.x < 1.01f) {
        if (selected_axis == 1) {
            out_color = vec3(1.0, 1.0, 0.0);
        } else {
            out_color = vec3(0.0, 1.0, 0.0);
        }
    } else if (in_texcoord.x < 2.01f) {
        if (selected_axis == 2) {
            out_color = vec3(1.0, 1.0, 0.0);
        } else {
            out_color = vec3(0.0, 0.0, 1.0);
        }
    } else {
        out_color = vec3(1.0, 1.0, 1.0);
    }
}