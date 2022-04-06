//
// Created by cwb on 2022/4/6.
//

#ifndef CPP_DEMO_TEXTUREHANDLE_H
#define CPP_DEMO_TEXTUREHANDLE_H

#include <stdint.h>
#include <string_view>

class TextureHandle
{
public:
    enum class PixelFormat
    {
        RGBA,
        RGB
    };

    struct Info
    {
        PixelFormat format = PixelFormat::RGBA;
        int width = 0;
        int height = 0;
        int channels = 0;
    };

public:
    TextureHandle() = default;
    virtual ~TextureHandle() = default;
    virtual bool createByData(void* data, uint32_t width, uint32_t height, PixelFormat format) = 0;
    virtual bool createByFile(std::string_view filePath) = 0;
    virtual void resize(uint32_t width, uint32_t height, const void* data) = 0;
    virtual uint64_t handle() = 0;
    inline int width() const
    {
        return m_info.width;
    }
    inline int height() const
    {
        return m_info.height;
    }

protected:
    Info m_info;
};

#endif //CPP_DEMO_TEXTUREHANDLE_H
