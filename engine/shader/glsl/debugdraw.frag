#version 450

//-----------------------------------------------
// 调试绘制
// 1. 计算像素的颜色
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

/* 输入数据 */
layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

/* 输出数据 */
layout(location = 0) out vec4 outColor;

/* uniform数据 */
layout(set = 0, binding = 2) uniform sampler2D texSampler;

/* 主函数 */
void main() {
    outColor = fragColor;
    if (fragTexCoord.x >= 0.0f && fragTexCoord.y >= 0.0f) {
        vec4  tex = texture(texSampler, fragTexCoord);
        float xi  = tex.r;
        outColor  = vec4(fragColor.r * xi, fragColor.g * xi, fragColor.b * xi, fragColor.a * xi);
    }
}