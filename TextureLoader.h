#pragma once
#include <string>
#include <GL/glew.h>

class TextureLoader
{
public:
    TextureLoader();
    GLuint getTexture(std::string texFileName);
    ~TextureLoader();
};
