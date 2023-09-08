#version 450

//-----------------------------------------------
// 调试绘制
// 1. 计算顶点位置、顶点大小
// 2. 输出片段颜色、纹理坐标
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* uniform 数据 */
layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 proj_view_matrix; // 投影视图矩阵
}
ubo;
layout(set = 0, binding = 1) uniform UniformDynamicBufferObject {
    mat4 model; // 模型矩阵
    vec4 color; // 颜色
}
dynamic_ubo;

/* 输入数据 */
layout(location = 0) in vec3 inPosition; // 顶点位置
layout(location = 1) in vec4 inColor;    // 顶点颜色
layout(location = 2) in vec2 texcoord;   // 纹理坐标

/* 输出数据 */
layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

/* 主函数 */
void main() {
    // 顶点位置
    if (texcoord.x < 0) {
        gl_Position = ubo.proj_view_matrix * dynamic_ubo.model * vec4(inPosition, 1.0);
    } else {
        gl_Position = vec4(inPosition, 1.0);
    }

    // 顶点大小
    gl_PointSize = 2;

    // 片段颜色
    if (dynamic_ubo.color.a > 0.000001) {
        fragColor = dynamic_ubo.color;
    } else {
        fragColor = inColor;
    }

    // 纹理坐标
    fragTexCoord = texcoord;
}