#version 310 es

//-----------------------------------------------
// 网格体选择: 好像就是把传入的数据直接传出去, 没有做任何处理
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"
#include "structures.vert"

/* uniform 数据 */
layout(set = 0, binding = 0) readonly buffer _unused_name_preframe {
    mat4 proj_view_matrix; // 投影视图矩阵
    uint rt_width;         // 渲染目标宽度
    uint rt_hight;         // 渲染目标高度
};
layout(set = 0, binding = 1) readonly buffer _unused_name_perdrawcall {
    mat4  model_matrices[m_mesh_per_drawcall_max_instance_count];          // 模型矩阵
    uint  node_ids[m_mesh_per_drawcall_max_instance_count];                // 节点ID
    float enable_vertex_blendings[m_mesh_per_drawcall_max_instance_count]; // 是否启用顶点混合
};
layout(set = 0, binding = 2) readonly buffer _unused_name_per_drawcall_vertex_blending {
    mat4 joint_matrices[m_mesh_vertex_blending_max_joint_count * m_mesh_per_drawcall_max_instance_count]; // 骨骼变换矩阵
};

layout(set = 1, binding = 0) readonly buffer _unused_name_per_mesh_joint_binding {
    VulkanMeshVertexJointBinding indices_and_weights[]; // 骨骼索引和权重
};

/* 输入数据 */
layout(location = 0) in vec3 in_position; // 顶点位置

/* 输出数据 */
layout(location = 0) flat out highp uint out_nodeid; // 节点ID, 使用flat修饰符, 保证在顶点着色器和片段着色器之间插值不会发生变化

/* 主函数 */
void main() {
    highp mat4  model_matrix           = model_matrix[gl_InstanceIndex];
    highp float enable_vertex_blending = enable_vertex_blending[gl_InstanceIndex];

    // 顶点在模型空间的位置
    highp vec3 model_position;

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
    }
    // 如果不需要顶点混合, 则直接使用顶点的输入(局部坐标)作为顶点最终的模型空间的位置、法线、切线
    else {
        model_position = in_position;
    }

    // 顶点位置
    gl_Position = proj_view_matrix * model_matrix * vec4(in_position, 1.0);

    // 节点ID
    out_nodeid = node_ids[gl_InstanceIndex];
}