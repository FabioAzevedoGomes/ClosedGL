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

void trilinearResample(Texture *texture, Vertex &fragment)
{
    // TODO: Mipmaping and trilinear resampling
}

void PassThroughLightingStrategy::ShadeFragmentRelativeToCamera(Vertex &fragment, Camera &camera)
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
                trilinearResample(texture, fragment);
                break;
        }
    }
}
