#include "Texture.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#endif

glm::vec<3, unsigned char> Mipmap::sample(int s, int t)
{
    unsigned char * offset = image + (s + (height * t )) * 4;
    return glm::vec<3, unsigned char>(
        offset[0],
        offset[1],
        offset[2]
    );
}

Texture::t_texture()
{
    this->image = nullptr;
    this->width = 0;
    this->height = 0;
    this->numChannels = 0;
    this->needsReload = false;
    this->mipmapCount = 0;
    this->mipmaps = nullptr;
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
    for (int i = 0; i < this->mipmapCount; i++) {
        free(this->mipmaps[i].image);
    }
    free(this->mipmaps);
    this->mipmapCount = 0;
}

std::string Texture::getName()
{
    return name.length() > 0 ? name : "None";
}

bool Texture::exists()
{
    return image != nullptr;
}

glm::vec3 Texture::sample(int s, int t)
{
    unsigned char * offset = image + (s + (height * t )) * 4;
    return glm::vec3(
        (float)offset[0]/255.0f,
        (float)offset[1]/255.0f,
        (float)offset[2]/255.0f
    );
}

void Texture::generateMipmap() 
{
    // Deal with resizing for power of two
    int pow2Width = std::pow(2, std::ceil(std::log2(width)));
    int pow2Height = std::pow(2, std::ceil(std::log2(height)));
    unsigned char *initialImage = nullptr;
    
    this->mipmapCount = 1;
    this->mipmaps = (Mipmap *)malloc(sizeof(Mipmap) * std::max(std::ceil(std::log2(pow2Width)),std::ceil(std::log2(pow2Height))));

    initialImage = (unsigned char *)malloc(sizeof(unsigned char) * pow2Width * pow2Height * 4);
    memset(initialImage, 0, sizeof(unsigned char) * pow2Width * pow2Height * 4);
    memcpy(initialImage, image,  pow2Width * pow2Height * 4 * sizeof(unsigned char));

    mipmaps[0] = {
        .image = initialImage,
        .width = pow2Width,
        .height = pow2Height,
    };

    // Generate pyramid
    int level;
    for (pow2Width /= 2, pow2Height /= 2, level = 1;
         pow2Width > 1 && pow2Height > 1;
         pow2Width /= 2, pow2Height /= 2, level++)
    {
        unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char) * pow2Width * pow2Height * 4);
        for (int i = 0; i + 2 < pow2Height * 2; i+=2) {
            for (int j = 0; j + 2 < pow2Width * 2; j+=2) {
                glm::vec3 color(0.0f);
                glm::vec3 sample = sampleMipmap(level - 1, j, i);
                color.x += sample.x / 4.0f;
                color.y += sample.y / 4.0f;
                color.z += sample.z / 4.0f;
                sample = sampleMipmap(level - 1, j, i);
                color.x += sample.x / 4.0f;
                color.y += sample.y / 4.0f;
                color.z += sample.z / 4.0f;
                sample = sampleMipmap(level - 1, j, i);
                color.x += sample.x / 4.0f;
                color.y += sample.y / 4.0f;
                color.z += sample.z / 4.0f;
                sample = sampleMipmap(level - 1, j, i);
                color.x += sample.x / 4.0f;
                color.y += sample.y / 4.0f;
                color.z += sample.z / 4.0f;

                (buffer + (j/2 + (i/2 * pow2Height)) * 4)[0] = color.x*255.0f;
                (buffer + (j/2 + (i/2 * pow2Height)) * 4)[1] = color.y*255.0f;
                (buffer + (j/2 + (i/2 * pow2Height)) * 4)[2] = color.z*255.0f;
                (buffer + (j/2 + (i/2 * pow2Height)) * 4)[3] = 255;
            }
        }

        mipmaps[level] = {
            .image = buffer,
            .width = pow2Width,
            .height = pow2Height,
        };
        this->mipmapCount++;
    }
}

glm::vec3 Texture::sampleMipmap(int level, int s, int t) 
{
    s = std::max(0.0f, (float)std::min(this->mipmaps[level].width, s));
    t = std::max(0.0f, (float)std::min(this->mipmaps[level].height, t));

    glm::vec<3, unsigned char> color = this->mipmaps[level].sample(s, t);
    return glm::vec3(
        (float)color.x/255.0f,
        (float)color.y/255.0f,
        (float)color.z/255.0f        
    );
}
