#pragma once

#include <iostream>

typedef struct t_texture
{
    unsigned char *image;
    int width, height, numChannels;

    t_texture();
    t_texture(const char *filename);
    ~t_texture();
    bool exists();

} Texture;
