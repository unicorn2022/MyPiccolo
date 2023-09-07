#version 310 es

//--------------------------------------
// 点光影阴影:
// 1. 输出顶点在世界空间中的标准位置
//--------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"
#include "structures.vert"

/* uniform 数据 */
layout(set = 0, binding = 1) readonly buffer _unused_name_per_drawcall {
    VulkanMeshInstance mesh_instances[m_mesh_per_drawcall_max_instance_count]; // 网格实例
};
layout(set = 0, binding = 2) readonly buffer _unused_name_per_drawcall_vertex_blending {
    mat4 joint_matrices[m_mesh_vertex_blending_max_joint_count * m_mesh_per_drawcall_max_instance_count]; // 骨骼变换矩阵
};

layout(set = 1, binding = 0) readonly buffer _unused_name_per_mesh_joint_binding {
    VulkanMeshVertexJointBinding indices_and_weights[]; // 骨骼索引和权重
};

/* 输入数据 */
layout(location = 0) in highp vec3 in_position;

/* 输出数据 */
layout(location = 0) out highp vec4 out_position_world_space;

/* 主函数 */
void main() {
    highp mat4  model_matrix           = mesh_instances[gl_InstanceIndex].model_matrix;
    highp float enable_vertex_blending = mesh_instances[gl_InstanceIndex].enable_vertex_blending;

    // 顶点在模型空间的位置
    highp vec3 model_position;

    // 如果需要顶点混合, 则将joint_matrix加权平均, 作用到输入(局部坐标)上, 得到顶点最终的模型空间的位置、法线、切线
    if (enable_vertex_blending > 0.0) {
        // 顶点对应的四个骨骼的索引&权重
        highp ivec4 in_indices = indices_and_weights[gl_InstanceIndex].indices;
        highp vec4  in_weights = indices_and_weights[gl_InstanceIndex].weights;

        // 顶点对应的四个骨骼的变换矩阵加权平均
        highp mat4 vertex_blending_matrix = mat4x4 {
            vec4(0.0, 0.0, 0.0, 0.0),
            vec4(0.0, 0.0, 0.0, 0.0),
            vec4(0.0, 0.0, 0.0, 0.0),
            vec4(0.0, 0.0, 0.0, 0.0)};

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

    } else {
        model_position = in_position;
    }

    // 顶点在世界坐标中的位置
    out_position_world_space = (model_matrix * vec4(model_position, 1.0)).xyz;
}