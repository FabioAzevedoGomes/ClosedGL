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
    
    initialImage = (unsigned char *)malloc(sizeof(unsigned char) * pow2Width * pow2Height * 4);
    memset(initialImage, 0, sizeof(unsigned char) * pow2Width * pow2Height * 4);
    memcpy_s(initialImage, pow2Width * pow2Height * 4, image, sizeof(unsigned char));

    mipmaps[0] = {
        .image = initialImage,
        .width = pow2Width,
        .height = pow2Height,
    };

    // Generate pyramid
    int level;
    for (pow2Width /= 2, pow2Height /= 2, level = 1;
         pow2Width > 1 && level < NUM_GENERATED_MIPMAPS;
         pow2Width /= 2, pow2Height /= 2, level++)
    {    
        unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char) * pow2Width * pow2Height * 4);
        for (int i = 0; i + 2 < pow2Height * 2; i+=2) {
            for (int j = 0; j + 2 < pow2Width * 2; j+=2) {
                glm::vec<3, unsigned char> color = glm::vec<3, unsigned char>(0.0f);
                for (int k = 0; k < 4; k++) {
                    glm::vec<3, unsigned char> sample = mipmaps[level - 1].sample(j, i);
                    color.x += sample.x / 4;
                    color.y += sample.y / 4;
                    color.z += sample.z / 4;
                }
                (buffer + (j/2 + (i/2 * pow2Height)) * 4)[0] = color.x;
                (buffer + (j/2 + (i/2 * pow2Height)) * 4)[1] = color.y;
                (buffer + (j/2 + (i/2 * pow2Height)) * 4)[2] = color.z;
                (buffer + (j/2 + (i/2 * pow2Height)) * 4)[3] = 255;
            }
        }

        mipmaps[level] = {
            .image = buffer,
            .width = pow2Width,
            .height = pow2Height,
        };
    }
}
