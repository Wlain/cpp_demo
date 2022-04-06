//
// Created by cwb on 2022/4/6.
//

#ifndef CPP_DEMO_TEXTUREGL_H
#define CPP_DEMO_TEXTUREGL_H

#include "textureHandle.h"

class TextureGL : public TextureHandle
{
public:
    ~TextureGL() override = default;
    bool createByData(void* data, uint32_t width, uint32_t height, PixelFormat format) override;
    bool createByFile(std::string_view filePath) override;
    void resize(uint32_t width, uint32_t height, const void* data) override;
    uint64_t handle() override;

public:
    uint32_t m_id = 0;
};

#endif //CPP_DEMO_TEXTUREGL_H
