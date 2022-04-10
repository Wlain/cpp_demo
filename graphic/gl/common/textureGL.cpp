//
// Created by cwb on 2022/4/6.
//

#include "textureGL.h"

#include "utils.h"

#include <SOIL2/stb_image.h>

TextureGL::~TextureGL()
{
    destroy();
}

void TextureGL::destroy()
{
    if (m_id != 0)
    {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}

bool TextureGL::createByData(void* data, uint32_t width, uint32_t height, TextureHandle::PixelFormat format)
{
    return false;
}

bool TextureGL::createByFile(std::string_view filePath)
{
    // load and create a texture
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrappingType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrappingType);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filteringType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filteringType);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.data(), &m_info.width, &m_info.height, &m_info.channels, 0);
    if (data)
    {
        auto format = GL_RGBA;
        switch (m_info.channels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            ASSERT(0);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_info.width, m_info.height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOG_ERROR("Failed to load texture");
        return false;
    }
    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);
    return true;
}

void TextureGL::resize(uint32_t width, uint32_t height, const void* data)
{
}

uint64_t TextureGL::handle()
{
    return m_id;
}
