//
// Created by william on 2022/5/18.
//

#include "renderHelp.h"
//
Bitmap::Bitmap(uint32_t width, uint32_t height) :
    m_width(width), m_height(height)
{
    m_pitch = m_width * 4;
    m_bits = new uint8_t[m_pitch * m_height];
    fill(0);
}

Bitmap::Bitmap(const Bitmap& src) :
    m_width(src.m_width), m_height(src.m_height)
{
    m_pitch = m_width * 4;
    m_bits = new uint8_t[m_pitch * m_height];
    memcpy(m_bits, src.m_bits, sizeof(uint8_t) * m_pitch * m_height);
}

Bitmap::Bitmap(const char* filename)
{
    auto* temp = loadFile(filename);
    if (temp == nullptr)
    {
        std::string msg = "load filed:";
        msg.append(filename);
        throw std::runtime_error(msg);
    }
    m_width = temp->m_width;
    m_height = temp->m_height;
    m_pitch = temp->m_pitch;
    m_bits = temp->m_bits;
    temp->m_bits = nullptr;
    delete temp;
}

Bitmap::~Bitmap()
{
    if (m_bits)
    {
        delete[] m_bits;
        m_bits = nullptr;
    }
}

void Bitmap::fill(uint32_t color)
{
    for (int j = 0; j < m_height; ++j)
    {
        auto* row = (uint32_t*)(m_bits + j * m_pitch);
        for (int i = 0; i < m_width; ++i, row++)
            memcpy(row, &color, sizeof(uint32_t));
    }
}

uint32_t Bitmap::getPixel(int x, int y) const
{
    uint32_t color = 0;
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        memcpy(&color, m_bits + y * m_pitch + x * 4, sizeof(uint32_t));
    }
    return color;
}

void Bitmap::setPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        memcpy(m_bits + y * m_pitch + x * 4, &color, sizeof(uint32_t));
    }
}

Bitmap* Bitmap::loadFile(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == nullptr) return nullptr;
    Info info;
    uint8_t header[14];
    int hr = (int)fread(header, 1, 14, fp);
    if (hr != 14)
    {
        fclose(fp);
        return nullptr;
    }
    if (header[0] != 0x42 || header[1] != 0x4d)
    {
        fclose(fp);
        return nullptr;
    }
    hr = (int)fread(&info, 1, sizeof(info), fp);
    if (hr != 40)
    {
        fclose(fp);
        return nullptr;
    }
    if (info.biBitCount != 24 && info.biBitCount != 32)
    {
        fclose(fp);
        return nullptr;
    }
    auto* bmp = new Bitmap(info.biWidth, info.biHeight);
    uint32_t offset;
    std::memcpy(&offset, header + 10, sizeof(uint32_t));
    fseek(fp, offset, SEEK_SET);
    uint32_t pixelSize = (info.biBitCount + 7) / 8;
    uint32_t pitch = (pixelSize * info.biWidth + 3) & (~3);
    for (int y = 0; y < (int)info.biHeight; y++)
    {
        uint8_t* line = bmp->getLine(info.biHeight - 1 - y);
        for (int x = 0; x < (int)info.biWidth; x++, line += 4)
        {
            line[3] = 255;
            fread(line, pixelSize, 1, fp);
        }
        fseek(fp, pitch - info.biWidth * pixelSize, SEEK_CUR);
    }
    fclose(fp);
    return bmp;
}

bool Bitmap::saveFile(const char* filename, bool withAlpha) const
{
    FILE* fp = fopen(filename, "wb");
    if (fp == nullptr) return false;
    Info info{};
    uint32_t pixelSize = (withAlpha) ? 4 : 3;
    uint32_t pitch = (m_width * pixelSize + 3) & (~3);
    info.biSizeImage = pitch * m_height;
    uint32_t bfSize = 54 + info.biSizeImage;
    uint32_t zero = 0, offset = 54;
    fputc(0x42, fp);
    fputc(0x4d, fp);
    fwrite(&bfSize, 4, 1, fp);
    fwrite(&zero, 4, 1, fp);
    fwrite(&offset, 4, 1, fp);
    info.biSize = 40;
    info.biWidth = m_width;
    info.biHeight = m_height;
    info.biPlanes = 1;
    info.biBitCount = (withAlpha) ? 32 : 24;
    info.biCompression = 0;
    info.biXPelsPerMeter = 0xb12;
    info.biYPelsPerMeter = 0xb12;
    info.biClrUsed = 0;
    info.biClrImportant = 0;
    fwrite(&info, sizeof(info), 1, fp);
    for (int y = 0; y < m_height; y++)
    {
        const uint8_t* line = getLine(info.biHeight - 1 - y);
        uint32_t padding = pitch - m_width * pixelSize;
        for (int x = 0; x < m_width; x++, line += 4)
        {
            fwrite(line, pixelSize, 1, fp);
        }
        for (int i = 0; i < (int)padding; i++)
            fputc(0, fp);
    }
    fclose(fp);
    return true;
}

uint32_t Bitmap::sampleBilinear(float x, float y) const
{
    auto fx = (int32_t)(x * 0x10000);
    auto fy = (int32_t)(y * 0x10000);
    int32_t x1 = std::clamp(fx >> 16, 0, m_width - 1);
    int32_t y1 = std::clamp(fy >> 16, 0, m_height - 1);
    int32_t x2 = std::clamp(x1 + 1, 0, m_width - 1);
    int32_t y2 = std::clamp(y1 + 1, 0, m_height - 1);
    int32_t dx = (fx >> 8) & 0xff;
    int32_t dy = (fy >> 8) & 0xff;
    if (m_width <= 0 || m_height <= 0) return 0;
    uint32_t c00 = getPixel(x1, y1);
    uint32_t c01 = getPixel(x2, y1);
    uint32_t c10 = getPixel(x1, y2);
    uint32_t c11 = getPixel(x2, y2);
    return bilinearInterp(c00, c01, c10, c11, dx, dy);
}

uint32_t Bitmap::bilinearInterp(uint32_t tl, uint32_t tr, uint32_t bl, uint32_t br, int32_t distx, int32_t disty)
{
    uint32_t f, r;
    int32_t distxy = distx * disty;
    int32_t distxiy = (distx << 8) - distxy; /* distx * (256 - disty) */
    int32_t distixy = (disty << 8) - distxy; /* disty * (256 - distx) */
    int32_t distixiy = 256 * 256 - (disty << 8) - (distx << 8) + distxy;
    r = (tl & 0x000000ff) * distixiy + (tr & 0x000000ff) * distxiy + (bl & 0x000000ff) * distixy + (br & 0x000000ff) * distxy;
    f = (tl & 0x0000ff00) * distixiy + (tr & 0x0000ff00) * distxiy + (bl & 0x0000ff00) * distixy + (br & 0x0000ff00) * distxy;
    r |= f & 0xff000000;
    tl >>= 16;
    tr >>= 16;
    bl >>= 16;
    br >>= 16;
    r >>= 16;
    f = (tl & 0x000000ff) * distixiy + (tr & 0x000000ff) * distxiy + (bl & 0x000000ff) * distixy + (br & 0x000000ff) * distxy;
    r |= f & 0x00ff0000;
    f = (tl & 0x0000ff00) * distixiy + (tr & 0x0000ff00) * distxiy + (bl & 0x0000ff00) * distixy + (br & 0x0000ff00) * distxy;
    r |= f & 0xff000000;
    return r;
}

void Bitmap::drawLine(int x1, int y1, int x2, int y2, uint32_t color)
{
    int x, y;
    if (x1 == x2 && y1 == y2)
    {
        setPixel(x1, y1, color);
        return;
    }
    else if (x1 == x2)
    {
        int inc = (y1 <= y2) ? 1 : -1;
        for (y = y1; y != y2; y += inc)
            setPixel(x1, y, color);
        setPixel(x2, y2, color);
    }
    else if (y1 == y2)
    {
        int inc = (x1 <= x2) ? 1 : -1;
        for (x = x1; x != x2; x += inc)
            setPixel(x, y1, color);
        setPixel(x2, y2, color);
    }
    else
    {
        int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
        int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
        int rem = 0;
        if (dx >= dy)
        {
            if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; x <= x2; x++)
            {
                setPixel(x, y, color);
                rem += dy;
                if (rem >= dx)
                {
                    rem -= dx;
                    y += (y2 >= y1) ? 1 : -1;
                    setPixel(x, y, color);
                }
            }
            setPixel(x2, y2, color);
        }
        else
        {
            if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; y <= y2; y++)
            {
                setPixel(x, y, color);
                rem += dx;
                if (rem >= dy)
                {
                    rem -= dy;
                    x += (x2 >= x1) ? 1 : -1;
                    setPixel(x, y, color);
                }
            }
            setPixel(x2, y2, color);
        }
    }
}

void Bitmap::flipHorizontal()
{
    for (int y = 0; y < m_height; y++)
    {
        for (int i = 0, j = m_width - 1; i < j; i++, j--)
        {
            uint32_t c1 = getPixel(i, y);
            uint32_t c2 = getPixel(j, y);
            setPixel(i, y, c2);
            setPixel(j, y, c1);
        }
    }
}

void Bitmap::flipVertical()
{
    auto* buffer = new uint8_t[m_pitch];
    for (int i = 0, j = m_height - 1; i < j; i++, j--)
    {
        memcpy(buffer, getLine(i), m_pitch);
        memcpy(getLine(i), getLine(j), m_pitch);
        memcpy(getLine(j), buffer, m_pitch);
    }
    delete[] buffer;
}

void Bitmap::setPixel(int x, int y, const Vec4f& color)
{
    setPixel(x, y, vectorToColor(color));
}

Vec4f Bitmap::sample2D(const Vec2f& uv) const
{
    return sample2D(uv.x, uv.y);
}

Vec4f Bitmap::sample2D(float u, float v) const
{
    uint32_t rgba = sampleBilinear(u * m_width + 0.5f, v * m_height + 0.5f);
    return vectorFromColor(rgba);
}

RenderHelp::RenderHelp() = default;

RenderHelp::RenderHelp(int32_t width, int32_t height)
{
    init(width, height);
}

RenderHelp::~RenderHelp()
{
    reset();
}

void RenderHelp::reset()
{
    m_vertexShader = nullptr;
    m_fragmentShader = nullptr;
    if (m_frameBuffer) delete m_frameBuffer;
    m_frameBuffer = nullptr;
    if (m_depthBuffer)
    {
        for (int j = 0; j < m_height; j++)
        {
            if (m_depthBuffer[j]) delete[] m_depthBuffer[j];
            m_depthBuffer[j] = nullptr;
        }
        delete[] m_depthBuffer;
        m_depthBuffer = nullptr;
    }
    m_colorFg = 0xffffffff;
    m_colorBg = 0xffffffff;
}

void RenderHelp::init(int32_t width, int32_t height)
{
    reset();
    m_width = width;
    m_height = height;
    m_frameBuffer = new Bitmap(width, height);
    m_depthBuffer = new float*[height];
    for (int j = 0; j < height; j++)
    {
        m_depthBuffer[j] = new float[width];
    }
    clear();
}

void RenderHelp::clear()
{
    if (m_frameBuffer)
    {
        m_frameBuffer->fill(m_colorBg);
    }
    if (m_depthBuffer)
    {
        for (int j = 0; j < m_height; j++)
        {
            for (int i = 0; i < m_width; i++)
                m_depthBuffer[j][i] = 0.0f;
        }
    }
}

bool RenderHelp::drawPrimitive()
{
    if (m_frameBuffer == nullptr || m_vertexShader == nullptr)
        return false;
    // 顶点初始化
    for (int i = 0; i < 3; ++i)
    {
        auto& vertex = m_vertices[i];
        // 清空 varing 列表
        vertex.context.varyingFloat.clear();
        vertex.context.varyingVec2f.clear();
        vertex.context.varyingVec3f.clear();
        vertex.context.varyingVec4f.clear();
        // 运行顶点着色器，返回顶点坐标
        vertex.pos = m_vertexShader(i, vertex.context);
        // 简单裁剪，任何一个顶点超过 CVV（Canonical View Volume,规则观察体，也叫做齐次裁剪空间） 就剔除
        float w = vertex.pos.w;
        /// early-z?
        // 这里图简单，当一个点越界，立马放弃整个三角形，更精细的做法是
        // 如果越界了就在齐次空间内进行裁剪，拆分为 0-2 个三角形然后继续
        if (w == 0.0f) return false;
        if (vertex.pos.z < 0.0f || vertex.pos.z > w) return false;
        if (vertex.pos.x < -w || vertex.pos.x > w) return false;
        if (vertex.pos.y < -w || vertex.pos.y > w) return false;

        // 计算 w 的倒数：Reciprocal of the Homogeneous W
        vertex.rhw = 1.0f / w;
        // 齐次坐标空间 /w 归一化到单位体积 cvv
        vertex.pos *= vertex.rhw;
        // 计算屏幕坐标
        vertex.spf.x = (vertex.pos.x + 1.0f) * m_width * 0.5f;
        vertex.spf.y = (1.0f - vertex.pos.y) * m_height * 0.5f;
        // 整数屏幕坐标：加 0.5 的偏移取屏幕像素方格中心对齐
        vertex.spi.x = (int)(vertex.spf.x + 0.5f);
        vertex.spi.y = (int)(vertex.spf.y + 0.5f);
        // 更新外接矩形范围
        if (i == 0)
        {
            m_range.left = m_range.right = std::clamp(vertex.spi.x, 0, m_width - 1);
            m_range.bottom = m_range.top = std::clamp(vertex.spi.y, 0, m_height - 1);
        }
        else
        {
            m_range.left = std::clamp(std::min(m_range.left, vertex.spi.x), 0, m_width - 1);
            m_range.right = std::clamp(std::max(m_range.right, vertex.spi.x), 0, m_width - 1);
            m_range.bottom = std::clamp(std::min(m_range.bottom, vertex.spi.y), 0, m_height - 1);
            m_range.top = std::clamp(std::max(m_range.top, vertex.spi.y), 0, m_height - 1);
        }
    }
    // 绘制线框
    if (m_renderFrame)
    {
        drawLine(m_vertices[0].spi.x, m_vertices[0].spi.y, m_vertices[1].spi.x, m_vertices[1].spi.y);
        drawLine(m_vertices[0].spi.x, m_vertices[0].spi.y, m_vertices[2].spi.x, m_vertices[2].spi.y);
        drawLine(m_vertices[2].spi.x, m_vertices[2].spi.y, m_vertices[1].spi.x, m_vertices[1].spi.y);
    }

    // 如果不填充像素就退出
    if (m_renderPixel == false) return false;
    // 判断三角形朝向
    Vec4f v01 = m_vertices[1].pos - m_vertices[0].pos;
    Vec4f v02 = m_vertices[2].pos - m_vertices[0].pos;
    Vec4f normal = vectorCross(v01, v02);
    // 使用 vtx 访问三个顶点，而不直接用 _vertex 访问，因为可能会调整顺序
    Vertex* vtx[3] = { &m_vertices[0], &m_vertices[1], &m_vertices[2] };
    // 如果背向视点，则交换顶点，保证 edge equation 判断的符号为正
    if (normal.z > 0.0f)
    {
        vtx[1] = &m_vertices[2];
        vtx[2] = &m_vertices[1];
    }
    else if (normal.z == 0.0f)
    {
        return false;
    }
    // 保存三个端点位置
    Vec2i p0 = vtx[0]->spi;
    Vec2i p1 = vtx[1]->spi;
    Vec2i p2 = vtx[2]->spi;
    // 计算面积，为零就退出
    // 叉乘求面积
    float s = (float)std::abs(vectorCross(p1 - p0, p2 - p0));
    if (s <= 0) return false;
    // 三角形填充时，左面和上面的边上的点需要包括，右方和下方边上的点不包括
    // 先判断是否是 TopLeft，判断出来后会和下方 Edge Equation 一起决策
    bool topLeft01 = isTopLeft(p0, p1);
    bool topLeft12 = isTopLeft(p1, p2);
    bool topLeft20 = isTopLeft(p2, p0);
    // 迭代三角形外接矩形的所有点
    for (int cy = m_range.bottom; cy <= m_range.top; ++cy)
    {
        for (int cx = m_range.left; cx <= m_range.right; ++cx)
        {
            Vec2f px = { (float)cx + 0.5f, (float)cy + 0.5f };
            // Edge Equation
            // 使用整数避免浮点误差，同时因为是左手系，所以符号取反
            int e01 = -(cx - p0.x) * (p1.y - p0.y) + (cy - p0.y) * (p1.x - p0.x);
            int e12 = -(cx - p1.x) * (p2.y - p1.y) + (cy - p1.y) * (p2.x - p1.x);
            int e20 = -(cx - p2.x) * (p0.y - p2.y) + (cy - p2.y) * (p0.x - p2.x);
            // 如果是左上边，用 E >= 0 判断合法，如果右下边就用 E > 0 判断合法
            // 这里通过引入一个误差 1 ，来将 < 0 和 <= 0 用一个式子表达
            if (e01 < (topLeft01 ? 0 : 1)) continue; // 在第一条边后面
            if (e12 < (topLeft12 ? 0 : 1)) continue; // 在第二条边后面
            if (e20 < (topLeft20 ? 0 : 1)) continue; // 在第三条边后面
            // 三个端点到当前点的矢量
            auto s0 = vtx[0]->spf - px;
            auto s1 = vtx[1]->spf - px;
            auto s2 = vtx[2]->spf - px;
            // 重心坐标系：计算内部子三角形面积 a / b / c
            float a = std::abs(vectorCross(s1, s2)); // 子三角形 Px-P1-P2 面积
            float b = std::abs(vectorCross(s2, s0)); // 子三角形 Px-P2-P0 面积
            float c = std::abs(vectorCross(s0, s1)); // 子三角形 Px-P0-P1 面积
            float s = a + b + c;                     // 大三角形 P0-P1-P2 面积
            if (s == 0.0f) continue;
            // 除以总面积，以保证：a + b + c = 1，方便用作插值系数
            a = a * (1.0f / s);
            b = b * (1.0f / s);
            c = c * (1.0f / s);
            // 计算当前点的 1/w，因 1/w 和屏幕空间呈线性关系，故直接重心插值
            float rhw = vtx[0]->rhw * a + vtx[1]->rhw * b + vtx[2]->rhw * c;
            // 进行深度测试
            if (rhw < m_depthBuffer[cy][cx]) continue;
            m_depthBuffer[cy][cx] = rhw; // 记录 1/w 到深度缓存
            // 还原当前像素的 w
            float w = 1.0f / ((rhw != 0.0f) ? rhw : 1.0f);
            // 计算三个顶点插值 varying 的系数
            // 先除以各自顶点的 w 然后进行屏幕空间插值然后再乘以当前 w
            float c0 = vtx[0]->rhw * a * w;
            float c1 = vtx[1]->rhw * b * w;
            float c2 = vtx[2]->rhw * c * w;
            // 准备为当前像素的各项 varying 进行插值
            ShaderContext input;
            ShaderContext& i0 = vtx[0]->context;
            ShaderContext& i1 = vtx[1]->context;
            ShaderContext& i2 = vtx[2]->context;
            // 插值各项 varying
            for (auto const& it : i0.varyingFloat)
            {
                auto key = it.first;
                auto f0 = i0.varyingFloat[key];
                auto f1 = i1.varyingFloat[key];
                auto f2 = i2.varyingFloat[key];
                input.varyingFloat[key] = c0 * f0 + c1 * f1 + c2 * f2;
            }
            for (auto const& it : i0.varyingVec2f)
            {
                auto key = it.first;
                const auto& f0 = i0.varyingVec2f[key];
                const auto& f1 = i1.varyingVec2f[key];
                const auto& f2 = i2.varyingVec2f[key];
                input.varyingVec2f[key] = c0 * f0 + c1 * f1 + c2 * f2;
            }
            for (auto const& it : i0.varyingVec3f)
            {
                auto key = it.first;
                const auto& f0 = i0.varyingVec3f[key];
                const auto& f1 = i1.varyingVec3f[key];
                const auto& f2 = i2.varyingVec3f[key];
                input.varyingVec3f[key] = c0 * f0 + c1 * f1 + c2 * f2;
            }
            for (auto const& it : i0.varyingVec4f)
            {
                auto key = it.first;
                const auto& f0 = i0.varyingVec4f[key];
                const auto& f1 = i1.varyingVec4f[key];
                const auto& f2 = i2.varyingVec4f[key];
                input.varyingVec4f[key] = c0 * f0 + c1 * f1 + c2 * f2;
            }
            // 执行像素着色器
            Vec4f color = { 0.0f, 0.0f, 0.0f, 0.0f };
            if (m_fragmentShader != nullptr)
            {
                color = m_fragmentShader(input);
            }
            // 绘制到 framebuffer 上，这里可以加判断，如果 PS 返回的颜色 alpha 分量
            // 小于等于零则放弃绘制，不过这样的话要把前面的更新深度缓存的代码挪下来，
            // 只有需要渲染的时候才更新深度
            m_frameBuffer->setPixel(cx, cy, color);
        }
    }
    // 绘制线框，再画一次避免覆盖
    if (m_renderFrame)
    {
        drawLine(m_vertices[0].spi.x, m_vertices[0].spi.y, m_vertices[1].spi.x, m_vertices[1].spi.y);
        drawLine(m_vertices[0].spi.x, m_vertices[0].spi.y, m_vertices[2].spi.x, m_vertices[2].spi.y);
        drawLine(m_vertices[2].spi.x, m_vertices[2].spi.y, m_vertices[1].spi.x, m_vertices[1].spi.y);
    }
    return true;
}
