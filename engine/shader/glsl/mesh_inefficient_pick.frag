#version 310 es

//-----------------------------------------------
// 网格体选择
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* 输入数据 */
layout(location = 0) flat in highp uint in_nodeid; // 节点ID
// 在片段着色器执行之前进行早期片段测试。当早期片段测试确定片段应该被丢弃时，将不会执行片段着色器，并且不会对帧缓冲进行写入操作，以节省处理资源
layout(early_fragment_tests) in;

/* 输出数据 */
layout(location = 0) out highp uint out_node_id;

/* 主函数 */
void main() {
    out_node_id = in_nodeid;
}