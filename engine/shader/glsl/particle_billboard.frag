#version 310 es

//-----------------------------------------------
// billboard 粒子效果: 始终朝向相机的面片
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"
#include "gbuffer.frag"

/* uniform 数据 */
layout(set = 0, binding = 2) uniform sampler2D spark_texture; // 粒子透明度纹理

/* 输入数据 */
layout(location = 0) in highp vec4 in_color;
layout(location = 1) in highp vec2 in_uv;

/* 输出数据 */
layout(location = 0) out highp vec4 out_scene_color; // 粒子颜色

/* 主函数 */
void main() {
    out_scene_color.xyz = 4.0f * texture(spark_texture, in_uv).r * in_color.xyz;
    out_scene_color.w   = texture(spark_texture, in_uv).r;
}