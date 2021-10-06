#include "Texture.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#endif

Texture::t_texture()
{
    this->image = nullptr;
    this->width = 0;
    this->height = 0;
    this->numChannels = 0;
    this->needsReload = false;
}

Texture::t_texture(const char *filename)
{
    this->image = stbi_load(
        filename,
        &(this->width),
        &(this->height),
        &(this->numChannels),
        STBI_rgb_alpha);
    this->needsReload = true;
    this->name = filename;

    if (image == nullptr)
    {
        std::cerr << "Unable to load texture " << filename << std::endl;
        this->image = nullptr;
        this->width = 0;
        this->height = 0;
        this->numChannels = 0;
        this->needsReload = false;
    }
}

Texture::~t_texture()
{
    stbi_image_free(image);
}

std::string Texture::getName()
{
    return name.length() > 0 ? name : "None";
}

bool Texture::exists()
{
    return image != nullptr;
}

void Texture::generateMipmap() 
{
    // TODO:
}
