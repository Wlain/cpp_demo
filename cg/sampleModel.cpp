//
// Created by william on 2022/5/19.
//
#include "commonMacro.h"
#include "model.h"
void sampleModelTest()
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
    auto matProjection = matrixSetPerspective(MATH_DEG_TO_RAD(45), 6 / 8.0f, 1.0, 500.0f);
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
    const int VARYING_NORMAL = 1;
    // 顶点着色器
    render.setVertexShader([&](int index, ShaderContext& output) {
        Vec4f pos = vertexAttrib[index].pos.xyz1() * matMvp;
        Vec4f normal = (vertexAttrib[index].normal.xyz1() * matModelInverse);
        output.varyingVec2f[VARYING_TEXCOORDS] = vertexAttrib[index].uv;
        output.varyingVec3f[VARYING_NORMAL] = normal.xyz(); // 转化为三维
        return pos;
    });
    // 片元着色器
    render.setFragmentShader([&](ShaderContext& input) {
        Vec2f uv = input.varyingVec2f[VARYING_TEXCOORDS];
        Vec3f n = input.varyingVec3f[VARYING_NORMAL];
        Vec3f l = vectorNormalize(lightDir);
        Vec4f color = model.diffuse(uv); // 从模型取得纹理颜色
        // 点乘 n,l 获得光照强度，范围限制在 [0,1]
        float intense = std::clamp(vectorDot(n, l), 0.0f, 1.0f) + 0.1f;
        return color * intense;
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
    render.saveFile(GET_CURRENT("/output/sampleModelTest.bmp"));
}