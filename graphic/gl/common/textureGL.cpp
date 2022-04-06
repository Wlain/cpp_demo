//
// Created by cwb on 2022/4/6.
//

#include "textureGL.h"

#include "base.h"

#include <SOIL2/stb_image.h>

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.data(), &m_info.width, &m_info.height, &m_info.channels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_info.width, m_info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOG_ERROR("Failed to load texture");
        return false;
    }
    stbi_image_free(data);
    return true;
}

void TextureGL::resize(uint32_t width, uint32_t height, const void* data)
{
}

uint64_t TextureGL::handle()
{
    return m_id;
}
