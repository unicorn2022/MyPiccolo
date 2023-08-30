/* Vulkan网格体实例 */
struct VulkanMeshInstance {
    highp float enable_vertex_blending;             // 顶点混合
    highp float _padding_enable_vertex_blending_1;  // 顶点混合对齐1
    highp float _padding_enable_vertex_blending_2;  // 顶点混合对齐2
    highp float _padding_enable_vertex_blending_3;  // 顶点混合对齐3
    
    highp mat4  model_matrix;   // 模型矩阵
};

/* Vulkan网格体顶点骨骼绑定 */
struct VulkanMeshVertexJointBinding {
    highp ivec4 indices;    // 索引
    highp vec4  weights;    // 权重
};
