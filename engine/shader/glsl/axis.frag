#version 460
//--------------------
//     坐标轴绘制
//--------------------

/* 输入数据 */
layout(location = 0) in vec3 in_color;

/* 输出数据 */
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(in_color, 1.0);
}