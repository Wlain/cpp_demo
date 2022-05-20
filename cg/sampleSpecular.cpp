//
// Created by william on 2022/5/20.
//
#include "commonMacro.h"
#include "model.h"

void sampleSpecularTest()
{
    RenderHelp render(600, 800);
    // 加载模型
    Model model(GET_CURRENT("/resources/objects/diablo3/diablo3_pose.obj"));
    Vec3f eyePos = { 0.0f, -0.5f, 1.7f };
    Vec3f target = { 0.0f, 0.0f, 0.0f };
    Vec3f up = { 0.0f, 1.0f, 0.0f };
    Vec3f lightDir = { 1.0f, 1.0f, 0.85f }; // 光的方向
    auto matModel = matrixSetScale(0.5f, 0.5f, 0.5f);
    auto matView = matrixSetLookat(eyePos, target, up);
    auto matProjection = matrixSetPerspective(MATH_DEG_TO_RAD(45), 600.0f / 800.0f, 1.0, 500.0f);
    auto matMvp = matModel * matView * matProjection;
    // model 矩阵求逆转置，用于将法向从模型坐标变换到世界坐标
    auto matModelInverse = matrixInvert(matModel).transpose();
    // 顶点属性
    struct VertexAttrib
    {
        Vec3f pos;
        Vec3f normal;
        Vec2f uv;
    };
    VertexAttrib vertexAttrib[3];
    const int VARYING_TEXCOORDS = 0;
    const int VARYING_WORLD_POS = 1; // 眼睛相对顶点的位置
    // 顶点着色器
    render.setVertexShader([&](int index, ShaderContext& output) {
        Vec4f pos = vertexAttrib[index].pos.xyz1() * matMvp;
        output.varyingVec2f[VARYING_TEXCOORDS] = vertexAttrib[index].uv;
        // 将顶点位置从模型空间转换为世界坐标系
        Vec3f worldPos = (vertexAttrib[index].pos.xyz1() * matModel).xyz();
        output.varyingVec3f[VARYING_WORLD_POS] = worldPos;
        return pos;
    });
    // 片元着色器
    render.setFragmentShader([&](ShaderContext& input) {
        Vec2f uv = input.varyingVec2f[VARYING_TEXCOORDS];
        Vec3f worldPos = input.varyingVec3f[VARYING_WORLD_POS];
        // 从模型中取出当前点的高光参数
        float ks = model.specular(uv);
        float ka = 0.1f;
        // 从模型读取每个点的法向并变换到世界坐标系
        Vec3f n = (model.normal(uv).xyz1() * matModelInverse).xyz();
        Vec3f l = vectorNormalize(lightDir);
        Vec4f color = model.diffuse(uv); // 从模型取得纹理颜色
        // 计算反射光线
        Vec3f reflected = vectorNormalize(n * vectorDot(n, l) * 2.0f - l);
        // 计算高光
        float p = std::clamp(vectorDot(reflected, worldPos), 0.0f, 1.0f);
        float spec = std::clamp(pow(p, ks * 20.0f) * 0.1f, 0.0f, 1.0f);
        // 综合光强
        float intensity = std::clamp(vectorDot(n, l), 0.0f, 1.0f) + ka + spec;
        return color * intensity;
    });
    // 迭代模型每一个面
    for (int i = 0; i < model.facesNum(); i++)
    {
        // 设置三个顶点的输入，供 VS 读取
        for (int j = 0; j < 3; j++)
        {
            vertexAttrib[j].pos = model.vert(i, j);
            vertexAttrib[j].uv = model.uv(i, j);
            vertexAttrib[j].normal = model.normal(i, j);
        }
        // 绘制三角形
        render.drawPrimitive();
    }
    // 保存结果
    render.saveFile(GET_CURRENT("/output/sampleSpecularTest.bmp"));
}