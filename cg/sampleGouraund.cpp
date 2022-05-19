//
// Created by william on 2022/5/19.
//
#include "commonMacro.h"
#include "renderHelp.h"

void sampleGouraundTest()
{
    // 定义属性和 varying 中的纹理坐标 key
    constexpr int VARYING_TEXCOORDS = 0;
    constexpr int VARYING_COLOR = 1;
    constexpr int VARYING_LIGHT = 2;
    // 定义顶点结构
    struct VertexAttrib
    {
        Vec3f pos;
        Vec2f uv;
        Vec3f color;
        Vec3f normal;
    };
    // 顶点着色器输入
    VertexAttrib vertexAttrib[3];

    // 模型
    // clang-format off
    VertexAttrib mesh[] = {
        { {  1, -1,  1, }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, },
        { { -1, -1,  1, }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, },
        { { -1,  1,  1, }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, },
        { {  1,  1,  1, }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, },
        { {  1, -1, -1, }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, },
        { { -1, -1, -1, }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, },
        { { -1,  1, -1, }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, },
        { {  1,  1, -1, }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, }
    };
    // clang-format on
    RenderHelp render(800, 600);
    // 定义一个纹理，并生成网格图案
    Bitmap texture(256, 256);
    for (int y = 0; y < 256; y++)
    {
        for (int x = 0; x < 256; x++)
        {
            int k = (x / 32 + y / 32) & 1;
            texture.setPixel(x, y, k ? 0xffffffff : 0xff3fbcef);
        }
    }
    auto matModel = matrixSetRotate(-1.0f, -0.5f, 1.0f, MATH_DEG_TO_RAD(60.0f));                      // 模型变换，旋转一定角度
    auto matView = matrixSetLookat({ 3.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }); // 摄像机方位
    auto matProjection = matrixSetPerspective(MATH_DEG_TO_RAD(45), 800 / 600.0f, 1.0, 500.0f);
    auto matMvp = matModel * matView * matProjection; // 综合变换矩阵
    // model 矩阵求逆转置，用于将法向从模型坐标变换到世界坐标
    auto matModelInverse = matrixInvert(matModel).transpose();
    // 光照方向
    Vec3f lightDir = { 1, 1, 2 };
    // 顶点着色器
    render.setVertexShader([&](int index, ShaderContext& output) {
        /// 左手坐标系
        auto pos = vertexAttrib[index].pos.xyz1() * matMvp;
        output.varyingVec2f[VARYING_TEXCOORDS] = vertexAttrib[index].uv;
        output.varyingVec3f[VARYING_COLOR] = vertexAttrib[index].color;
        // 法向需要经过 model 矩阵的逆矩阵转置的矩阵变换，从模型坐标系转换
        // 到世界坐标系，光照需要在世界坐标系内进行运算
        auto normal = vertexAttrib[index].normal;
        normal = (normal.xyz1() * matModelInverse).xyz();
        // 计算光照强度
        auto intensity = std::max(0.0f, vectorDot(normal, vectorNormalize(lightDir)));
        // 加入一个常量环境光 0.1
        intensity += 0.1f;
        output.varyingFloat[VARYING_LIGHT] = intensity;
        return pos;
    });
    // 片元着色器
    render.setFragmentShader([&](ShaderContext& input) {
        Vec2f texCoords = input.varyingVec2f[VARYING_TEXCOORDS]; // 取得纹理坐标
        Vec4f color = texture.sample2D(texCoords);
        color = color * input.varyingFloat[VARYING_LIGHT];
        return color;
    });

    auto drawPlane = [&](RenderHelp& render, int a, int b, int c, int d) {
        mesh[a].uv.x = 0;
        mesh[a].uv.y = 0;
        mesh[b].uv.x = 0;
        mesh[b].uv.y = 1;
        mesh[c].uv.x = 1;
        mesh[c].uv.y = 1;
        mesh[d].uv.x = 1;
        mesh[d].uv.y = 0;
        auto ab = mesh[b].pos - mesh[a].pos;
        auto ac = mesh[c].pos - mesh[a].pos;
        auto normal = vectorNormalize(vectorCross(ac, ab));
        mesh[a].normal = mesh[b].normal = mesh[c].normal = mesh[d].normal = normal;
        vertexAttrib[0] = mesh[a];
        vertexAttrib[1] = mesh[b];
        vertexAttrib[2] = mesh[c];
        render.drawPrimitive();
        vertexAttrib[0] = mesh[c];
        vertexAttrib[1] = mesh[d];
        vertexAttrib[2] = mesh[a];
        render.drawPrimitive();
    };

    // 绘制盒子
    drawPlane(render, 0, 1, 2, 3);
    drawPlane(render, 7, 6, 5, 4);
    drawPlane(render, 0, 4, 5, 1);
    drawPlane(render, 1, 5, 6, 2);
    drawPlane(render, 2, 6, 7, 3);
    drawPlane(render, 3, 7, 4, 0);
    // 保存结果
    render.saveFile(GET_CURRENT("/output/sampleGouraundTest.bmp"));
}