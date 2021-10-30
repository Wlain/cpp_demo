//
// Created by ytech on 2021/10/29.
//

#ifndef CPP_DEMO_BUFFER_H
#define CPP_DEMO_BUFFER_H
#include <eigen3/Eigen/Eigen>
#include <stdint.h>

namespace graphics
{
enum class Buffers : uint32_t
{
    Color = 1,
    Depth = 2,
};

inline Buffers operator|(Buffers a, Buffers b)
{
    return Buffers((uint32_t)a | (uint32_t)b);
}

inline Buffers operator&(Buffers a, Buffers b)
{
    return Buffers((uint32_t)a & (uint32_t)b);
}

enum class Primitive : uint32_t
{
    Line,
    Triangle,
    Triangle_Line
};

struct PositionBufferHandle
{
    int posHandle = 0;
};

struct IndicesBufferHandle
{
    int indicesHandle = 0;
};

struct ColorBufferHandle
{
    int colorHandle = 0;
};

class BufferData
{
public:
    BufferData();
    ~BufferData();
    BufferData(BufferData&&);
    BufferData(const BufferData&);

    PositionBufferHandle loadPositions(const std::vector<Eigen::Vector4f>& position);
    IndicesBufferHandle loadIndices(const std::vector<Eigen::Vector3i>& indices);
    ColorBufferHandle loadColors(std::vector<Eigen::Vector4f>& colors);

    inline std::map<int, std::vector<Eigen::Vector4f>>& positionBuf() { return m_positionBuf; }
    inline std::map<int, std::vector<Eigen::Vector3i>>& indicesBuf() { return m_indicesBuf; }
    inline std::map<int, std::vector<Eigen::Vector4f>>& colorBuf() { return m_colorBuf; }
    inline std::vector<Eigen::Vector3f>& frameBuffer() { return m_frameBuffer; }
    inline std::vector<Eigen::Vector3f>& frameBufferForMsaa() { return m_frameBufferForMsaa; }
    inline std::shared_ptr<Eigen::Vector3f>& resolveColor() { return m_resolveColor; }
    inline std::vector<float>& depthBuffer()  { return m_depthBuffer; }
    inline int getNextId() { return m_nextID++; }
private:
    std::map<int, std::vector<Eigen::Vector4f>> m_positionBuf;
    std::map<int, std::vector<Eigen::Vector3i>> m_indicesBuf;
    std::map<int, std::vector<Eigen::Vector4f>> m_colorBuf;
    std::vector<Eigen::Vector3f> m_frameBuffer;
    std::vector<Eigen::Vector3f> m_frameBufferForMsaa;
    std::shared_ptr<Eigen::Vector3f> m_resolveColor;
    std::vector<float> m_depthBuffer;
    int m_nextID = 0;
};

} // namespace graphics

#endif //CPP_DEMO_BUFFER_H
