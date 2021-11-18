//
// Created by ytech on 2021/10/29.
//

#include "buffer.h"

#include "base.h"

using namespace Eigen;
namespace graphics
{
BufferData::BufferData()
{
    LOG_INFO("BufferData constructor called");
}
BufferData::~BufferData()
{
    LOG_INFO("BufferData constructor called");
}
BufferData::BufferData(BufferData&&)
{
    LOG_INFO("BufferData moved constructor called");
}

BufferData::BufferData(const BufferData&)
{
    LOG_INFO("BufferData cpoy constructor called");
}

PositionBufferHandle BufferData::loadPositions(const std::vector<Vector4f>& position)
{
    auto id = getNextId();
    m_positionBuf.emplace(id, position);
    return { id };
}

IndicesBufferHandle BufferData::loadIndices(const std::vector<Vector3i>& indices)
{
    auto id = getNextId();
    m_indicesBuf.emplace(id, indices);
    return { id };
}

ColorBufferHandle BufferData::loadColors(std::vector<Vector4f>& colors)
{
    auto id = getNextId();
    m_colorBuf.emplace(id, colors);
    return { id };
}
} // namespace graphics
