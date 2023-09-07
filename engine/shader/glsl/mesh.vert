#version 310 es

//----------------------------------------------------------
// 蒙皮网格体绘制:
// 1. 计算当前顶点在相机空间中的标准位置
// 2. 输出顶点的世界空间的位置、法线、切线、纹理坐标
//----------------------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"
#include "structures.vert"

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
layout(set = 0, binding = 1) readonly buffer _unused_name_per_drawcall {
    VulkanMeshInstance mesh_instances[m_mesh_per_drawcall_max_instance_count]; // 网格实例
};
layout(set = 0, binding = 2) readonly buffer _unused_name_per_drawcall_vertex_blending {
    highp mat4 joint_matrices[m_mesh_vertex_blending_max_joint_count * m_mesh_per_drawcall_max_instance_count]; // 骨骼变换矩阵
};
layout(set = 1, binding = 0) readonly buffer _unused_name_per_mesh_joint_binding {
    VulkanMeshVertexJointBinding indices_and_weights[]; // 骨骼索引和权重
};

/* 输入数据 */
layout(location = 0) in vec3 in_position; // 位置
layout(location = 1) in vec3 in_normal;   // 法线
layout(location = 2) in vec3 in_tangent;  // 切线
layout(location = 3) in vec2 in_texcoord; // 纹理坐标

/* 输出数据 */
layout(location = 0) out vec3 out_world_position; // 世界坐标
layout(location = 1) out vec3 out_normal;         // 法线
layout(location = 2) out vec3 out_tagent;         // 切线
layout(location = 3) out vec2 out_texcoord;       // 纹理坐标

void main() {
    highp mat4  model_matrix           = mesh_instances[gl_InstanceIndex].model_matrix;
    highp float enable_vertex_blending = mesh_instances[gl_InstanceIndex].enable_vertex_blending;

    // 顶点在模型空间的位置、法线、切线
    highp vec3 model_position;
    highp vec3 model_normal;
    highp vec3 model_tangent;

    // 如果需要顶点混合, 则将joint_matrix加权平均, 作用到输入(局部坐标)上, 得到顶点最终的模型空间的位置、法线、切线
    if (enable_vertex_blending > 0.0) {
        highp ivec4 in_indices = indices_and_weights[gl_VertexIndex].indices;
        highp vec4  in_weights = indices_and_weights[gl_VertexIndex].weights;

        highp mat4 vertex_blending_matrix = mat4x4(
            vec4(0.0, 0.0, 0.0, 0.0),
            vec4(0.0, 0.0, 0.0, 0.0),
            vec4(0.0, 0.0, 0.0, 0.0),
            vec4(0.0, 0.0, 0.0, 0.0));

        if (in_weights.x > 0.0 && in_indices.x > 0) {
            vertex_blending_matrix += joint_matrices[m_mesh_vertex_blending_max_joint_count * gl_InstanceIndex + in_indices.x] * in_weights.x;
        }
        if (in_weights.y > 0.0 && in_indices.y > 0) {
            vertex_blending_matrix += joint_matrices[m_mesh_vertex_blending_max_joint_count * gl_InstanceIndex + in_indices.y] * in_weights.y;
        }
        if (in_weights.z > 0.0 && in_indices.z > 0) {
            vertex_blending_matrix += joint_matrices[m_mesh_vertex_blending_max_joint_count * gl_InstanceIndex + in_indices.z] * in_weights.z;
        }
        if (in_weights.w > 0.0 && in_indices.w > 0) {
            vertex_blending_matrix += joint_matrices[m_mesh_vertex_blending_max_joint_count * gl_InstanceIndex + in_indices.w] * in_weights.w;
        }

        model_position = (vertex_blending_matrix * vec4(in_position, 1.0)).xyz;

        highp mat3x3 vertex_blending_tangend_matrix = mat3x3(
            vertex_blending_matrix[0].xyz,
            vertex_blending_matrix[1].xyz,
            vertex_blending_matrix[2].xyz);

        model_normal  = normalize(vertex_blending_tangend_matrix * in_normal);
        model_tangent = normalize(vertex_blending_tangend_matrix * in_tangent);
    }
    // 如果不需要顶点混合, 则直接使用顶点的输入(局部坐标)作为顶点最终的模型空间的位置、法线、切线
    else {
        model_position = in_position;
        model_normal   = in_normal;
        model_tangent  = in_tangent;
    }

    mat3x3 tangend_matrix = mat3x3(
        model_matrix[0].xyz,
        model_matrix[1].xyz,
        model_matrix[2].xyz);

    // 计算世界空间下的位置、法线、切线、纹理坐标
    out_world_position = (model_matrix * vec4(model_position, 1.0)).xyz;
    out_normal         = normalize(tangend_matrix * model_normal);
    out_tagent         = normalize(tangend_matrix * model_tangent);
    out_texcoord       = in_texcoord;

    // 当前顶点在相机空间中的位置
    gl_Position = proj_view_matrix * vec4(out_world_position, 1.0f);
}