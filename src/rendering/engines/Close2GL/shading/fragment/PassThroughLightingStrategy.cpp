#include "PassThroughLightingStrategy.hpp"

void nearestNeighborResample(Texture *texture, Vertex &fragment) 
{
    float s = fragment.texture_coords.x * texture->width;
    float t = fragment.texture_coords.y * texture->height;
    fragment.color = texture->sample((int)std::round(s), (int)std::round(t));

}

void bilinearResample(Texture *texture, Vertex &fragment)
{
    float s = fragment.texture_coords.x * texture->width;
    float t = fragment.texture_coords.y * texture->height;
    float w11 = ((std::ceil(s) - s) * (std::ceil(t) - t));
    float w12 = ((std::ceil(s) - s) * (t - std::floor(t)));
    float w21 = ((s - std::floor(s)) * (std::ceil(t) - t));
    float w22 = ((s - std::floor(s)) * (t - std::floor(t)));
    fragment.color = (w11 * texture->sample((int)std::ceil(s), (int)std::ceil(t)))
                    + (w12 * texture->sample((int)std::ceil(s), (int)std::floor(t)))
                    + (w21 * texture->sample((int)std::floor(s), (int)std::ceil(t)))
                    + (w22 * texture->sample((int)std::floor(s), (int) std::floor(t)));
}

void trilinearResample(Texture *texture, Vertex &fragment, Buffer *buffers)
{
    if (fragment.position.x < 0 || fragment.position.x >= buffers->width || fragment.position.y < 0 || fragment.position.y >= buffers->height)
        return;

    float back_s  = *(buffers->stBuffer + ((int)std::floor(std::max(fragment.position.y, 0.0f)) * buffers->width * 2) + ((int)std::floor(std::max(fragment.position.x - 1, 0.0f)) * 2) + 0);
    float top_t = *(buffers->stBuffer + ((int)std::floor(std::max(fragment.position.y - 1, 0.0f)) * buffers->width * 2) + ((int)std::floor(std::max(fragment.position.x, 0.0f)) * 2) + 1);

    float delta_s = std::abs(back_s - fragment.texture_coords.x);
    float delta_t = std::abs(top_t - fragment.texture_coords.y);

    float level = 0;
    if (delta_s != 0.0f || delta_t != 0.0f) {
        level = (std::min((float)texture->mipmapCount, std::max(1.0f, std::log2(std::max(delta_s * texture->mipmaps[0].width, delta_t * texture->mipmaps[0].height)))) - 1.0f);
    }
    int lowerLevel = std::floor(level);
    int upperLevel = std::ceil(level);

    float lowerS = fragment.texture_coords.s * texture->mipmaps[lowerLevel].width;
    float lowerT = fragment.texture_coords.t * texture->mipmaps[lowerLevel].height;

    float w11Lower = (std::ceil(lowerS) - lowerS) * (std::ceil(lowerT) - lowerT);
    float w12Lower = (std::ceil(lowerS) - lowerS) * (lowerT - std::floor(lowerT));
    float w21Lower = ((lowerS - std::floor(lowerS)) * (std::ceil(lowerT) - lowerT));
    float w22Lower = ((lowerS - std::floor(lowerS)) * (lowerT - std::floor(lowerT)));

    glm::vec3 colorLower = (w11Lower * texture->sampleMipmap(lowerLevel, (int)std::ceil(lowerS), (int)std::ceil(lowerT)))
                         + (w12Lower * texture->sampleMipmap(lowerLevel, (int)std::ceil(lowerS), (int)std::floor(lowerT)))
                         + (w21Lower * texture->sampleMipmap(lowerLevel, (int)std::floor(lowerS), (int)std::ceil(lowerT)))
                         + (w22Lower * texture->sampleMipmap(lowerLevel, (int)std::floor(lowerS), (int) std::floor(lowerT)));

    float upperS = fragment.texture_coords.s * texture->mipmaps[upperLevel].width;
    float upperT = fragment.texture_coords.t * texture->mipmaps[upperLevel].height;
    float w11Upper = (std::ceil(upperS) - upperS) * (std::ceil(upperT) - upperT);
    float w12Upper = (std::ceil(upperS) - upperS) * (upperT - std::floor(upperT));
    float w21Upper = ((upperS - std::floor(upperS)) * (std::ceil(upperT) - upperT));
    float w22Upper = ((upperS - std::floor(upperS)) * (upperT - std::floor(upperT)));

    glm::vec3 colorUpper = (w11Upper * texture->sampleMipmap(upperLevel, (int)std::ceil(upperS), (int)std::ceil(upperT)))
                         + (w12Upper * texture->sampleMipmap(upperLevel, (int)std::ceil(upperS), (int)std::floor(upperT)))
                         + (w21Upper * texture->sampleMipmap(upperLevel, (int)std::floor(upperS), (int)std::ceil(upperT)))
                         + (w22Upper * texture->sampleMipmap(upperLevel, (int)std::floor(upperS), (int) std::floor(upperT)));

    float alpha = level - lowerLevel; 
    fragment.color = (alpha * colorLower) + ((1 - alpha) * colorUpper);
}

void PassThroughLightingStrategy::ShadeFragmentRelativeToCamera(Vertex &fragment, Camera &camera, Buffer *buffers)
{
    unsigned char *offset = nullptr;
    fragment.color /= fragment.wp;
    fragment.normal /= fragment.wp;

    if (textureEnabled) {
        fragment.texture_coords /= fragment.wp;
        fragment.texture_coords.x = std::min(1.0f, std::max(0.0f, fragment.texture_coords.x));
        fragment.texture_coords.y = std::min(1.0f, std::max(0.0f, fragment.texture_coords.y));

        switch(resamplingMode)
        {
            case NearestNeighbor:
                nearestNeighborResample(texture, fragment);
                break;
            case Bilinear:
                bilinearResample(texture, fragment);
                break;
            case Trilinear:
                trilinearResample(texture, fragment, buffers);
                break;
        }
    }
}
