#pragma once

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <limits>
#include <cmath>
#include <iostream>

#define COLOR_CHANNELS 4
#define DEFALUT_DEPTH_BUFFER_VALUE std::numeric_limits<float>::max()

typedef struct t_buffer
{
    float *colorBuffer, *depthBuffer;
    float *stBuffer;
    int width, height;

    t_buffer(int newWidth, int newHeight)
    {
        colorBuffer = new float[newWidth * newHeight * COLOR_CHANNELS];
        memset(colorBuffer, 0.0f, sizeof(float) * newWidth * newHeight * COLOR_CHANNELS);

        depthBuffer = new float[newWidth * newHeight];
        for (int i = 0; i < newWidth; i++)
            for (int j = 0; j < newHeight; j++)
                *(depthBuffer + j * width + i) = DEFALUT_DEPTH_BUFFER_VALUE;

        stBuffer = new float[newWidth * newHeight * 2];
        memset(stBuffer, 0.0f, sizeof(float) * newWidth * newHeight * 2);

        width = newWidth;
        height = newHeight;
    }

    ~t_buffer()
    {
        delete[] colorBuffer;
        delete[] depthBuffer;
        delete[] stBuffer;
    }

    void resize(int newWidth, int newHeight)
    {
        delete[] colorBuffer;
        colorBuffer = new float[newWidth * newHeight * COLOR_CHANNELS];
        memset(colorBuffer, 0, sizeof(float) * newWidth * newHeight * COLOR_CHANNELS);

        delete[] depthBuffer;
        depthBuffer = new float[newWidth * newHeight];
        for (int i = 0; i < newWidth; i++)
            for (int j = 0; j < newHeight; j++)
                *(depthBuffer + j * newWidth + i) = DEFALUT_DEPTH_BUFFER_VALUE;

        delete[] stBuffer;
        stBuffer = new float[newWidth * newHeight * 2];
        memset(stBuffer, 0.0f, sizeof(float) * newWidth * newHeight * 2);

        width = newWidth;
        height = newHeight;
    }

    void setClearColor(glm::vec3 color)
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                *(colorBuffer + j * width * COLOR_CHANNELS + i * COLOR_CHANNELS + 0) = color.x;
                *(colorBuffer + j * width * COLOR_CHANNELS + i * COLOR_CHANNELS + 1) = color.y;
                *(colorBuffer + j * width * COLOR_CHANNELS + i * COLOR_CHANNELS + 2) = color.z;
                *(colorBuffer + j * width * COLOR_CHANNELS + i * COLOR_CHANNELS + 3) = 1.0f;
            }
        }
    }

    void draw(glm::vec4 position, glm::vec4 color)
    {

        if (position.x >= 0 && position.x < width && position.y >= 0 && position.y < height && position.z < *(depthBuffer + (int)std::floor(position.y) * width + (int)std::floor(position.x)))
        {
            *(depthBuffer + (int)std::floor(position.y) * width + (int)std::floor(position.x)) = position.z;
            *(colorBuffer + (int)std::floor(position.y) * width * 4 + (int)std::floor(position.x) * 4 + 0) = std::min(1.0f, std::max(color.x, 0.0f));
            *(colorBuffer + (int)std::floor(position.y) * width * 4 + (int)std::floor(position.x) * 4 + 1) = std::min(1.0f, std::max(color.y, 0.0f));
            *(colorBuffer + (int)std::floor(position.y) * width * 4 + (int)std::floor(position.x) * 4 + 2) = std::min(1.0f, std::max(color.z, 0.0f));
            *(colorBuffer + (int)std::floor(position.y) * width * 4 + (int)std::floor(position.x) * 4 + 3) = std::min(1.0f, std::max(color.w, 0.0f));
        }
    }

    void clearST()
    {
        delete[] stBuffer;
        stBuffer = new float[width * height * 2];
        memset(stBuffer, 0.0f, sizeof(float) * width * height * 2);
    }

    void updateST(glm::vec4 position, glm::vec2 st) 
    {
        if (position.x >= 0 && position.x < width && position.y >= 0 && position.y < height && position.z <= *(depthBuffer + (int)std::floor(position.y) * width + (int)std::floor(position.x)))
        {
            //std::cout << "Writing to " << (int)std::floor(position.y) << ", " << (int)std::floor(position.x) << std::endl;
            *(stBuffer + (int)std::floor(position.y) * width * 2 + (int)std::floor(position.x) * 2 + 0) = std::min(1.0f, std::max(st.x, 0.0f));
            *(stBuffer + (int)std::floor(position.y) * width * 2 + (int)std::floor(position.x) * 2 + 1) = std::min(1.0f, std::max(st.y, 0.0f));            
        }
    }

    unsigned char *getColorBuffer()
    {
        unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * width * height * COLOR_CHANNELS);
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                for (int k = 0; k < 4; k++)
                    *(buffer + (height - j - 1) * width * COLOR_CHANNELS + i * COLOR_CHANNELS + k) = (int)(*(colorBuffer + j * width * COLOR_CHANNELS + i * COLOR_CHANNELS + k) * 255);

        return buffer;
    }

} Buffer;
