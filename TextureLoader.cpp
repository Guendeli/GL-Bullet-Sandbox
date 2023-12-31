﻿#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Dependencies/stb-master/stb_image.h"

TextureLoader::TextureLoader()
{
    
}

TextureLoader::~TextureLoader()
{
    
}

GLuint TextureLoader::getTexture(std::string texFileName)
{
    int width, height, channels;

    stbi_uc* image = stbi_load(texFileName.c_str(), &width, &height, &channels, STBI_rgb);

    GLuint m_texture;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(image);

    return m_texture;
}


