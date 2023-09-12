#version 310 es

//-----------------------------------------------
// billboard 粒子效果: 始终朝向相机的面片
//  1. 计算面片上的顶点对应的位置
//  2. 输出顶点的纹理坐标、颜色
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"

/* 粒子数据 */
struct Particle {
    vec3  pos;    // 位置
    float life;   // 生命周期
    vec3  vel;    // 速度
    float size_x; // 大小
    vec3  acc;    // 加速度
    float size_y; // 大小
    vec4  color;  // 颜色
};

/* uniform 数据 */
layout(set = 0, binding = 0) uniform _unused_name_perframe {
    mat4 proj_view_matrix;  // 投影视图矩阵
    vec3 right_direction;   // 右方向
    vec3 up_direction;      // 上方向
    vec3 forward_direction; // 前方向
};
layout(set = 0, binding = 1) readonly buffer _unused_name_perdrawcall {
    Particle particles[]; // 粒子数据
};

/* 输出数据 */
layout(location = 0) out vec4 out_color; // 颜色
layout(location = 1) out vec2 out_uv;    // 纹理坐标

/* 主函数 */
void main() {
    // billboard对应的正方形的四个顶点位置
    const vec2 vertex_buffer[4] = vec2[4](
        vec2(-0.5, 0.5),
        vec2(0.5, 0.5),
        vec2(-0.5, -0.5),
        vec2(0.5, -0.5));
    // billboard对应的正方形的四个纹理坐标
    const vec2 uv_buffer[4] = vec2[4](
        vec2(0, 1),
        vec2(1, 1),
        vec2(0, 0),
        vec2(1, 0));
    // 当前顶点的位置
    vec2 model_position = vertex_buffer[gl_VertexIndex];

    // 获取粒子发射器的属性
    Particle particle = particles[gl_InstanceIndex];
    // 粒子发射器的位置
    vec3 anchor_location = particle.pos;

    /* billboard需要始终朝向摄像机 */
    // 速度分解
    vec3  vel_dir       = particle.vel;
    float project_vel_x = dot(vel_dir, right_direction);
    float project_vel_y = dot(vel_dir, up_direction);
    // 大小分解
    float size_x = particle.size_x;
    float size_y = particle.size_y;

    // 当前顶点的世界坐标 = 发射器坐标 + 当前顶点在XY轴方向上的长度
    vec3 world_position;
    if (abs(project_vel_x) < size_x || abs(project_vel_y) < size_y) {
        world_position = size_x * right_direction * model_position.x + size_y * up_direction * model_position.y + anchor_location;
    } else {
        vec3 project_dir = normalize(project_vel_x * right_direction + project_vel_y * up_direction);
        vec3 side_dir    = normalize(cross(forward_direction, project_dir));
        world_position   = size_x * side_dir * model_position.x + size_y * project_dir * model_position.y + anchor_location;
    }

    // 转化为ndc空间
    gl_Position = proj_view_matrix * vec4(world_position, 1.0);

    // 输出颜色、纹理坐标
    out_color = particle.color;
    out_uv    = uv_buffer[gl_VertexIndex];
}