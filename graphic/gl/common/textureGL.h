//
// Created by cwb on 2022/4/6.
//

#ifndef CPP_DEMO_TEXTUREGL_H
#define CPP_DEMO_TEXTUREGL_H

#include "base.h"
#include "textureHandle.h"

class TextureGL : public TextureHandle
{
public:
    ~TextureGL() override;
    void destroy() override;
    bool createByData(void* data, uint32_t width, uint32_t height, PixelFormat format) override;
    bool createByFile(std::string_view filePath) override;
    void resize(uint32_t width, uint32_t height, const void* data) override;
    uint64_t handle() override;
    inline void setWarpingType(int32_t type)
    {
        m_wrappingType = type;
    }
    inline void setFilteringType(int32_t type)
    {
        m_filteringType = type;
    }

public:
    uint32_t m_id = 0;
    int32_t m_wrappingType = GL_CLAMP_TO_EDGE;
    int32_t m_filteringType = GL_LINEAR;
};

#endif //CPP_DEMO_TEXTUREGL_H
