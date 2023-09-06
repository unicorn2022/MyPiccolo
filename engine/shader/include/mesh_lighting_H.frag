#define PI 3.1416

#define MAX_REFLECTION_LOD 8.0

// 计算BRDF的法线分布项(D)
highp float D_GGX(highp float dotNH, highp float roughness) {
    highp float alpha  = roughness * roughness;
    highp float alpha2 = alpha * alpha;
    highp float denom  = dotNH * dotNH * (alpha2 - 1.0) + 1.0;
    return (alpha2) / (PI * denom * denom);
}

highp float Pow5(highp float x) {
    return (x * x * x * x * x);
}

// 计算BRDF的菲涅尔项(F)
highp vec3 F_Schlick(highp float cosTheta, highp vec3 F0) {
    return F0 + (1.0 - F0) * Pow5(1 - cosTheta);
}

// 计算BRDF的菲涅尔项(F), 引入roughness
highp vec3 F_SchlickR(highp float cosTheta, highp vec3 F0, highp float roughness) {
    return F0 + (max(vec3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * Pow5(1 - cosTheta);
}

// 计算BRDF的几何遮挡项(G)
highp float G_SchlicksmithGGX(highp float dotNL, highp float dotNV, highp float roughness) {
    highp float r     = (roughness + 1.0);
    highp float k     = (r * r) / 8.0;
    highp float GGX_L = dotNL / (dotNL * (1.0 - k) + k);
    highp float GGX_V = dotNV / (dotNV * (1.0 - k) + k);
    return GGX_L * GGX_V;
}

// 计算BRDF项, 包含高光与漫反射
highp vec3 BRDF(
    highp vec3  L,         // 入射光方向
    highp vec3  V,         // 视线方向
    highp vec3  N,         // 法线方向
    highp vec3  F0,        // 菲涅尔项
    highp float basecolor, // 漫反射颜色
    highp float metallic,  // 金属度
    highp float roughness  // 粗糙度
) {
    /* 提前计算向量与点乘结果 */
    highp vec3  H     = normalize(V + L);           // 半程向量
    highp float dotNV = clamp(dot(N, V), 0.0, 1.0); // 视线与法线的夹角
    highp float dotNL = clamp(dot(N, L), 0.0, 1.0); // 入射光与法线的夹角
    highp float dotLH = clamp(dot(L, H), 0.0, 1.0); // 入射光与半程向量的夹角
    highp float dotNH = clamp(dot(N, H), 0.0, 1.0); // 法线与半程向量的夹角

    highp vec3 color = vec3(0.0);

    highp vec3 rroughness = max(0.05, roughness);
    // 计算BRDF的法线分布项(D)
    highp float D = D_GGX(dotNH, rroughness);
    // 计算BRDF的菲涅尔项(F)
    highp vec3 F = F_Schlick(dotNV, F0);
    // 计算BRDF的几何遮挡项(G)
    highp float G = G_SchlicksmithGGX(dotNL, dotNV, rroughness);

    // 计算BRDF的高光项
    highp vec3 specular = D * F * G / (4.0 * dotNL * dotNV + 0.001);
    // 计算BRDF的漫反射项
    highp vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);

    color += (kD * basecolor / PI + (1.0 - kD) * specular);
    return color;
}

// ndc标准空间xy[-1,1] => uv[0,1]
highp vec2 ndcxy_to_uv(highp vec2 ndcxy) {
    return ndcxy * vec2(0.5, 0.5) + vec2(0.5, 0.5);
}

// uv[0,1] => ndc标准空间xy[-1,1]
highp vec2 uv_to_ndcxy(highp vec2 uv) {
    return uv * vec2(2.0, 2.0) + vec2(-1.0, -1.0);
}