#include "PassThroughLightingStrategy.hpp"

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
                offset = texture->image + (((int)std::round(fragment.texture_coords.x * (texture->width-1))) + (texture->height * ((int)std::round((fragment.texture_coords.y * (texture->height-1)))))) * 4;
                fragment.color = glm::vec3(
                    (float)offset[0]/255.0f,
                    (float)offset[1]/255.0f,
                    (float)offset[2]/255.0f
                );
                break;
            case Bilinear:
            
                break;
            case Trilinear:
            
                break;
        }
    }
}
