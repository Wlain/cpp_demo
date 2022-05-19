//
// Created by william on 2022/5/18.
//

#include "commonMacro.h"
#include "renderHelp.h"

void sampleTriangleTest()
{
    // 初始化渲染器和帧缓存大小
    RenderHelp render(800, 600);
    constexpr const int VARYING_COLOR = 0; // 定义一个 varying 的 key
    // 顶点数据，由 VS 读取，如有多个三角形，可每次更新 vs_input 再绘制
    struct VertexAttrib
    {
        Vec4f pos;
        Vec4f color;
    };
    VertexAttrib vertex[] = {
        { { 0.0, 0.7, 0.90, 1 }, { 0.85f, 0.93f, 0.72f, 1.0f } },
        { { -0.6, -0.2, 0.01, 1 }, { 0.85f, 0.93f, 0.72f, 1.0f } },
        { { +0.6, -0.2, 0.01, 1 }, { 0.85f, 0.93f, 0.72f, 1.0f } }
    };
    // 顶点着色器，初始化 varying 并返回坐标，
    // 参数 index 是渲染器传入的顶点序号，范围 [0, 2] 用于读取顶点数据
    render.setVertexShader([&](int index, ShaderContext& output) {
        output.varyingVec4f[VARYING_COLOR] = vertex[index].color;
        return vertex[index].pos; // 直接返回坐标
    });

    // 像素着色器，返回颜色
    render.setFragmentShader([&](ShaderContext& input) {
        return input.varyingVec4f[VARYING_COLOR];
    });
    render.setBGColor(0xffffffff);
    render.setFGColor(0xffff0000);
    render.setRenderState(true, true);
    // 渲染并保存
    render.drawPrimitive();
    render.saveFile(GET_CURRENT("/output/sampleTriangleTest.bmp"));
}
