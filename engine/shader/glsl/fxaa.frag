#version 310 es

//-----------------------------------------------
// 快速近似抗锯齿: 仅在边界处进行抗锯齿处理
// (a) 十字滤波计算边界
// (b) 卷积计算横向 or 竖向差距更大, 找到offset方向
// (c) 双向搜索, 通过判断当前像素对的颜色差与初始像素对的颜色差是否相同, 找到当前边界的两个端点
// (d) 比较当前像素与两个端点的距离, 计算混合权重 magnitude
// (e) 通过周围点的平均亮度, 修正 magnitude
// (f) 根据 magnitude 更改采样点的位置, 进行插值
//-----------------------------------------------

#extension GL_GOOGLE_include_directive : enable

#include "constants.vert"
precision highp float;
precision highp int;

/* uniform 数据 */
layout(set = 0, binding = 0) uniform sampler2D in_color; // 单帧图像

/* 输入数据 */
layout(location = 0) in vec2 in_uv; // 纹理坐标

/* 输出数据 */
layout(location = 0) out vec4 out_color; // 颜色

//  3*3卷积核中的像素索引
//    +---+---+---+
//    | 0 | 1 | 2 |
//    +---+---+---+
//    | 3 | 4 | 5 |
//    +---+---+---+
//    | 6 | 7 | 8 |
//    +---+---+---+
#define UP_LEFT 0
#define UP 1
#define UP_RIGHT 2
#define LEFT 3
#define CENTER 4
#define RIGHT 5
#define DOWN_LEFT 6
#define DOWN 7
#define DOWN_RIGHT 8
vec2 KERNEL_STEP_MAT[] = vec2[9](vec2(-1.0, 1.0), vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(-1.0, 0.0), vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(-1.0, -1.0), vec2(0.0, -1.0), vec2(1.0, -1.0));

//  为了加速沿切线的双向探索, 逐步增加跨越的像素数量 QUALITY(i), 最大搜索12次
//    +-----------------+---+---+---+---+---+---+---+---+---+---+---+---+
//    |step index       | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |11 |
//    +-----------------+---+---+---+---+---+---+---+---+---+---+---+---+
//    |step pixels count|1.0|1.0|1.0|1.0|1.0|1.5|2.0|2.0|2.0|2.0|4.0|8.0|
//    +-----------------+---+---+---+---+---+---+---+---+---+---+---+---+
#define STEP_COUNT_MAX 12
float QUALITY(int i) {
    if (i < 5) return 1.0;
    if (i == 5) return 1.5;
    if (i < 10) return 2.0;
    if (i == 10) return 4.0;
    if (i == 11) return 8.0;
}

// 计算RGB对应的亮度: L = 0.299 * R + 0.587 * G + 0.114 * B
float RGB2LUMA(vec3 color) {
    return dot(color, vec3(0.299, 0.587, 0.114));
}

#define EDGE_THRESHOLD_MIN 0.0312 // 阈值: 进行FXAA的最小像素亮度差
#define EDGE_THRESHOLD_MAX 0.125  // 阈值: 进行FXAA的最小像素亮度差占比
#define SUBPIXEL_QUALITY 0.75     // 子像素质量
#define GRADIENT_SCALE 0.25       // 阈值: 边界的梯度缩放

/* 主函数 */
void main() {
    // 屏幕尺寸
    highp ivec2 screen_size = textureSize(in_color, 0);
    // uv步长
    highp vec2 uv_step = vec2(1.0 / float(screen_size.x), 1.0 / float(screen_size.y));

    // 计算相邻像素的亮度
    float luma_mat[9];
    for (int i = 0; i < 9; i++) {
        luma_mat[i] = RGB2LUMA(texture(in_color, in_uv + uv_step * KERNEL_STEP_MAT[i]).xyz);
    }

    // --------------------------------------------------------------------------
    // 十字滤波计算边界
    // --------------------------------------------------------------------------
    /* 根据相邻颜色的亮度差, 判断当前像素是否在边界, 如果不是, 不进行FXAA计算 */
    float luma_min   = min(luma_mat[CENTER], min(min(luma_mat[UP], luma_mat[DOWN]), min(luma_mat[LEFT], luma_mat[RIGHT])));
    float luma_max   = max(luma_mat[CENTER], max(max(luma_mat[UP], luma_mat[DOWN]), max(luma_mat[LEFT], luma_mat[RIGHT])));
    float luma_range = luma_max - luma_min;
    if (luma_range < max(EDGE_THRESHOLD_MIN, luma_max * EDGE_THRESHOLD_MAX)) {
        out_color = texture(in_color, in_uv);
        return;
    }

    // --------------------------------------------------------------------------
    // 卷积计算横向 or 竖向差距更大, 找到offset方向
    // (1) offset方向: 与当前点颜色差距最大的方向
    // (2) step_tangend:   edge 方向移动的单位长度
    // (3) step_normal :  offset方向移动的单位长度
    // --------------------------------------------------------------------------
    /* 判断边界切线方向, 并计算step向量 */
    // 切线为水平的权重: |1 -2 1|, |2 -4 2|, |1 -2 1|
    float luma_horizontal = abs(luma_mat[UP_LEFT] + luma_mat[DOWN_LEFT] - 2.0 * luma_mat[LEFT]) + 2.0 * abs(luma_mat[UP] + luma_mat[DOWN] - 2.0 * luma_mat[CENTER]) + abs(luma_mat[UP_RIGHT] + luma_mat[DOWN_RIGHT] - 2.0 * luma_mat[RIGHT]);
    // 切线为垂直的权重: |1 -2 1|, |2 -4 2|, |1 -2 1|
    float luma_vertical = abs(luma_mat[UP_LEFT] + luma_mat[UP_RIGHT] - 2.0 * luma_mat[UP]) + 2.0 * abs(luma_mat[LEFT] + luma_mat[RIGHT] - 2.0 * luma_mat[CENTER]) + abs(luma_mat[DOWN_LEFT] + luma_mat[DOWN_RIGHT] - 2.0 * luma_mat[DOWN]);
    // 切线是否为水平
    bool is_horizontal = luma_horizontal > luma_vertical;
    // 切线step向量
    vec2 step_tangent = (is_horizontal ? vec2(1.0, 0.0) : vec2(0.0, 1.0)) * uv_step;

    /* 判断边界法线方向(offset朝向), 并计算step向量, 亮度变化梯度 */
    // 向下or左的梯度
    float gradient_down_left = (is_horizontal ? luma_mat[DOWN] : luma_mat[LEFT]) - luma_mat[CENTER];
    // 向上or右的梯度
    float gradient_up_right = (is_horizontal ? luma_mat[UP] : luma_mat[RIGHT]) - luma_mat[CENTER];
    // 法线是否向下or左
    bool is_down_left = abs(gradient_down_left) > abs(gradient_up_right);
    // 法线step向量
    vec2 step_normal = (is_down_left ? -1.0 : 1.0) * (is_horizontal ? vec2(0.0, 1.0) : vec2(1.0, 0.0)) * uv_step;
    // 计算法线方向的梯度
    float gradient = is_down_left ? gradient_down_left : gradient_up_right;

    // --------------------------------------------------------------------------
    // 双向搜索, 通过判断当前像素对的颜色差与初始像素对的颜色差是否相同, 找到当前边界的两个端点
    //  (1) L_avg: 初始像素对的平均亮度, L_contrast: 初始像素对的亮度差
    //  (2) L_pos: 正方向像素对的平均亮度, L_neg: 负方向像素对的平均亮度
    //  (3) 边界条件: abs(L_pos - L_avg) > 0.25 L_contrast
    //  (4) 平均亮度的计算: 通过采样中间点的颜色, 计算平均值
    // --------------------------------------------------------------------------
    /* 从当前点开始计算 */
    // 相邻两个像素的中点uv坐标
    vec2 uv_start = in_uv + 0.5 * step_normal;
    // 相邻两个像素的中点亮度
    float luma_average_start = luma_mat[CENTER] + 0.5 * gradient;

    /* 沿着切线开始双向搜索, 直到到达边界 */
    // 两个方向起始点的uv坐标
    vec2 uv_pos = uv_start + step_tangent;
    vec2 uv_neg = uv_start - step_tangent;
    // 两个方向起始点的亮度与平均亮度的差值
    float delta_luma_pos = RGB2LUMA(texture(in_color, uv_pos).rgb) - luma_average_start;
    float delta_luma_neg = RGB2LUMA(texture(in_color, uv_neg).rgb) - luma_average_start;
    // 是否到达了边界
    bool reached_pos  = abs(delta_luma_pos) > GRADIENT_SCALE * abs(gradient);
    bool reached_neg  = abs(delta_luma_neg) > GRADIENT_SCALE * abs(gradient);
    bool reached_both = reached_pos && reached_neg;
    // 如果没有, 则沿着切线前进一步
    if (!reached_pos) uv_pos += step_tangent;
    if (!reached_neg) uv_neg -= step_tangent;
    // 如果没有同时到达边界, 则进行循环计算
    if (!reached_both) {
        for (int i = 2; i < STEP_COUNT_MAX; i++) {
            if (!reached_pos) delta_luma_pos = RGB2LUMA(texture(in_color, uv_pos).rgb) - luma_average_start;
            if (!reached_neg) delta_luma_neg = RGB2LUMA(texture(in_color, uv_neg).rgb) - luma_average_start;

            bool reached_pos  = abs(delta_luma_pos) > GRADIENT_SCALE * abs(gradient);
            bool reached_neg  = abs(delta_luma_neg) > GRADIENT_SCALE * abs(gradient);
            bool reached_both = reached_pos && reached_neg;

            if (!reached_pos) uv_pos += (QUALITY(i) * step_tangent);
            if (!reached_neg) uv_neg -= (QUALITY(i) * step_tangent);

            if (reached_both) break;
        }
    }

    // --------------------------------------------------------------------------
    // 比较当前像素与两个端点的距离, 计算混合权重 magnitude
    //  (1) dst: 当前点到最近端点的距离
    //  (2) edgeLength: 整个边界的长度
    //  (3) magnitude:  与相邻像素的混合权重,
    //      如果 (L_avg - L_current) * (L_avg - L_targetP) > 0, 则 magnitude = 0
    //      否则 magnitude = abs(0.5 - dst / edgeLength)
    // --------------------------------------------------------------------------
    /* 计算两个边界点的距离 */
    float length_pos  = max(abs(uv_pos - uv_start).x, abs(uv_pos - uv_start).y);
    float length_neg  = max(abs(uv_neg - uv_start).x, abs(uv_neg - uv_start).y);
    bool  is_pos_near = length_pos < length_neg;
    // 计算 magnitude
    float pixel_offset = -1.0 * (is_pos_near ? length_pos : length_neg) / (length_pos + length_neg) + 0.5;
    if (((is_pos_near ? delta_luma_pos : delta_luma_neg) < 0.0) == (luma_mat[CENTER] < luma_average_start)) pixel_offset = 0.0;

    // --------------------------------------------------------------------------
    // 通过周围点的平均亮度, 修正 magnitude
    // --------------------------------------------------------------------------
    // 计算周围点的平均亮度
    float luma_average_center  = 0.0;
    float average_weight_mat[] = float[9](1.0, 2.0, 1.0, 2.0, 0.0, 2.0, 1.0, 2.0, 1.0);
    for (int i = 0; i < 9; i++) luma_average_center += average_weight_mat[i] * luma_mat[i];
    luma_average_center /= 12.0;
    // 计算子像素
    float subpixel_luma_range = clamp(abs(luma_average_center - luma_mat[CENTER]) / luma_range, 0.0, 1.0);
    float subpixel_offset     = (-2.0 * subpixel_luma_range + 3.0) * subpixel_luma_range * subpixel_luma_range;
    subpixel_offset           = subpixel_offset * subpixel_offset * SUBPIXEL_QUALITY;
    // 使用子像素偏移量修正magnitude
    pixel_offset = max(pixel_offset, subpixel_offset);

    // --------------------------------------------------------------------------
    // 根据 magnitude 更改采样点的位置, 进行插值
    // --------------------------------------------------------------------------
    out_color = texture(in_color, in_uv + pixel_offset * step_normal);
}