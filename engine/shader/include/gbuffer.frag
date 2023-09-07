struct PGBufferData {
    highp vec3  worldNormal;    // 法线
    highp vec3  baseColor;      // 基础颜色
    highp float metallic;       // 金属度
    highp float specular;       // 高光度
    highp float roughness;      // 粗糙度
    highp uint  shadingModelID; // 着色模型ID
};

// 着色模式: unLit, defaultLit
#define SHADINGMODELID_UNLIT 0U
#define SHADINGMODELID_DEFAULT_LIT 1U

// 法线
highp vec3 EncodeNormal(highp vec3 N) { return N * 0.5 + 0.5; }
highp vec3 DecodeNormal(highp vec3 N) { return N * 2.0 - 1.0; }

// 在渲染目标上使用sRGB，以提高深色的精度
highp vec3 EncodeBaseColor(highp vec3 baseColor) { return baseColor; }
highp vec3 DecodeBaseColor(highp vec3 baseColor) { return baseColor; }

// 着色模型ID
highp float EncodeShadingModelId(highp uint ShadingModelId) {
    highp uint Value = ShadingModelId;
    return (float(Value) / float(255));
}
highp uint DecodeShadingModelId(highp float InPackedChannel) {
    return uint(round(InPackedChannel * float(255)));
}

// GBuffer数据
void EncodeGBufferData(
    PGBufferData   InGBuffer,
    out highp vec4 OutGBufferA,
    out highp vec4 OutGBufferB,
    out highp vec4 OutGBufferC) {

    OutGBufferA.rgb = EncodeNormal(InGBuffer.worldNormal);

    OutGBufferB.r = InGBuffer.metallic;
    OutGBufferB.g = InGBuffer.specular;
    OutGBufferB.b = InGBuffer.roughness;
    OutGBufferB.a = EncodeShadingModelId(InGBuffer.shadingModelID);

    OutGBufferC.rgb = EncodeBaseColor(InGBuffer.baseColor);
}
void DecodeGBufferData(
    out PGBufferData OutGBuffer,
    highp vec4       InGBufferA,
    highp vec4       InGBufferB,
    highp vec4       InGBufferC) {

    OutGBuffer.worldNormal = DecodeNormal(InGBufferA.xyz);

    OutGBuffer.metallic       = InGBufferB.r;
    OutGBuffer.specular       = InGBufferB.g;
    OutGBuffer.roughness      = InGBufferB.b;
    OutGBuffer.shadingModelID = DecodeShadingModelId(InGBufferB.a);

    OutGBuffer.baseColor = DecodeBaseColor(InGBufferC.rgb);
}