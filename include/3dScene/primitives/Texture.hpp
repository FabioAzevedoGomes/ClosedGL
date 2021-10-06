#pragma once

#include <iostream>

#define NUM_GENERATED_MIPMAPS 8

typedef struct t_texture
{
    unsigned char *image;
    std::string name;
    int width, height, numChannels;
    bool needsReload;

    t_texture();
    t_texture(const char *filename);
    ~t_texture();
    std::string getName();
    bool exists();
    void generateMipmap();

} Texture;
