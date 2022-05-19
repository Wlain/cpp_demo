//
// Created by william on 2022/5/19.
//
#include "commonMacro.h"
#include "renderHelp.h"

void sampleTextureTest()
{
    constexpr uint32_t width{ 800 };
    constexpr uint32_t height{ 600 };
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
    auto matModel = Mat4x4f::identity();
    auto matView = matrixSetLookat({ 0.0f, 0.0f, 1.5f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
    auto matProjection = matrixSetPerspective(MATH_DEG_TO_RAD(45), (float)width / (float)height, 0.1f, 500.0f);
    auto matMvp = matModel * matView * matProjection;

    // 定义顶点输入
    struct VertexAttrib
    {
        Vec4f pos;
        Vec2f texCoords;
    };
    VertexAttrib vertexAttrib[3];
    // 定义属性和 varying 中的纹理坐标 key
    constexpr int VARYING_TEXCOORDS = 0;
    // 顶点着色器
    render.setVertexShader([&](int index, ShaderContext& output) {
        Vec4f pos = vertexAttrib[index].pos * matMvp; // 输出变换后的坐标
        output.varyingVec2f[VARYING_TEXCOORDS] = vertexAttrib[index].texCoords;
        return pos;
    });

    // 片元着色器
    render.setFragmentShader([&](ShaderContext& input) {
        auto coord = input.varyingVec2f[VARYING_TEXCOORDS]; // 取得纹理坐标
        return texture.sample2D(coord);
    });

    VertexAttrib vertex[] = {
        { { 1, -1, -1, 1}, {0, 0} },
        { { 1,  1, -1, 1}, {1, 0} },
        { {-1,  1, -1, 1}, {1, 1} },
        { {-1, -1, -1, 1}, {0, 1} },
    };
    render.setBGColor(0xffffffff);
    render.setFGColor(0xffff0000);
    render.setRenderState(true, true);

    vertexAttrib[0] = vertex[0];
    vertexAttrib[1] = vertex[1];
    vertexAttrib[2] = vertex[2];
    render.drawPrimitive();
    vertexAttrib[0] = vertex[2];
    vertexAttrib[1] = vertex[3];
    vertexAttrib[2] = vertex[0];
    render.drawPrimitive();
    render.saveFile(GET_CURRENT("/output/sampleTextureTest.bmp"));
}