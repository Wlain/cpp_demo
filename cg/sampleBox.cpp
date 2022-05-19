//
// Created by william on 2022/5/19.
//
#include "commonMacro.h"
#include "renderHelp.h"

void sampleBoxTest()
{
    // 定义顶点结构
    struct VertexAttrib
    {
        Vec3f pos;
        Vec2f uv;
        Vec3f color;
    };

    // 顶点着色器输入
    VertexAttrib vertexAttrib[3];
    // 模型
    // clang-format off
    VertexAttrib mesh[8] = {
        { {  1.0f, -1.0f,  1.0f, }, { 0.0f, 0.0f }, { 1.0f, 0.2f, 0.2f }},
        { { -1.0f, -1.0f,  1.0f, }, { 0.0f, 1.0f }, { 0.2f, 1.0f, 0.2f }},
        { { -1.0f,  1.0f,  1.0f, }, { 1.0f, 1.0f }, { 0.2f, 0.2f, 1.0f }},
        { {  1.0f,  1.0f,  1.0f, }, { 1.0f, 0.0f }, { 1.0f, 0.2f, 1.0f }},
        { {  1.0f, -1.0f, -1.0f, }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 0.2f }},
        { { -1.0f, -1.0f, -1.0f, }, { 0.0f, 1.0f }, { 0.2f, 1.0f, 1.0f }},
        { { -1.0f,  1.0f, -1.0f, }, { 1.0f, 1.0f }, { 1.0f, 0.3f, 0.3f }},
        { {  1.0f,  1.0f, -1.0f, }, { 1.0f, 0.0f }, { 0.2f, 1.0f, 0.3f }}
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
    // 定义属性和 varying 中的纹理坐标 key
    constexpr const int VARYING_TEXCOORDS = 0;
    constexpr const int VARYING_COLOR = 1;
    // 定义变换矩阵：模型变换，摄像机变换，透视变换
    auto matModel = matrixSetRotate(-1.0f, -0.5f, 1.0f, MATH_DEG_TO_RAD(60.0f));                      // 模型变换，旋转一定角度
    auto matView = matrixSetLookat({ 3.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }); // 摄像机方位
    auto matProjection = matrixSetPerspective(3.1415926f * 0.5f, 800 / 600.0, 1.0, 500.0f);
    auto matMvp = matModel * matView * matProjection; // 综合变换矩阵

    // 顶点着色器
    render.setVertexShader([&](int index, ShaderContext& output) {
        auto pos = vertexAttrib[index].pos.xyz1() * matMvp; // 扩充成四维矢量并变换
        output.varyingVec2f[VARYING_TEXCOORDS] = vertexAttrib[index].uv;
        output.varyingVec4f[VARYING_COLOR] = vertexAttrib[index].color.xyz1();
        return pos;
    });

    // 片元着色器
    render.setFragmentShader([&](ShaderContext& input) -> Vec4f {
        auto coord = input.varyingVec2f[VARYING_TEXCOORDS]; // 取得纹理坐标
        auto color = texture.sample2D(coord);               // 纹理采样并返回像素颜色
        return color;                                       // 返回纹理
    });
    render.setBGColor(0xffffffff);
    render.setFGColor(0xffff0000);
    render.setRenderState(true, true);

    auto drawPlane = [&](RenderHelp& rh, int a, int b, int c, int d) {
        mesh[a].uv.x = 0;
        mesh[a].uv.y = 0;
        mesh[b].uv.x = 0;
        mesh[b].uv.y = 1;
        mesh[c].uv.x = 1;
        mesh[c].uv.y = 1;
        mesh[d].uv.x = 1;
        mesh[d].uv.y = 0;
        vertexAttrib[0] = mesh[a];
        vertexAttrib[1] = mesh[b];
        vertexAttrib[2] = mesh[c];
        rh.drawPrimitive();
        vertexAttrib[0] = mesh[c];
        vertexAttrib[1] = mesh[d];
        vertexAttrib[2] = mesh[a];
        rh.drawPrimitive();
    };

    // 绘制盒子
    drawPlane(render, 0, 1, 2, 3);
    drawPlane(render, 7, 6, 5, 4);
    drawPlane(render, 0, 4, 5, 1);
    drawPlane(render, 1, 5, 6, 2);
    drawPlane(render, 2, 6, 7, 3);
    drawPlane(render, 3, 7, 4, 0);
    // 保存结果
    render.saveFile(GET_CURRENT("/output/sampleBoxTest.bmp"));
}