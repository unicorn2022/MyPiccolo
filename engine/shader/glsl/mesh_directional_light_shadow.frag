#version 310 es

//--------------------------------------
// 定向光影阴影:
// 1. 计算光源时间, 在当前像素看到的深度值
//--------------------------------------

/* 输入数据 */
layout(early_fragment_tests) in;

/* 输出数据 */
layout(location = 0) out highp float out_depth;

/* 主函数 */
void main() {
    out_depth = gl_FragCoord.z;
}