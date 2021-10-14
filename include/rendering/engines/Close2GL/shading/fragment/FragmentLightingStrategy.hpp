#pragma once

#include "Camera.hpp"
#include "Vertex.hpp"
#include "Material.hpp"
#include "ShadingConstants.hpp"
#include "Buffer.hpp"

#include <map>

class FragmentLightingStrategy
{
protected:
    Material uniformMaterial;
    float width, height;

    Texture *texture;
    bool textureEnabled;
    ResamplingModes resamplingMode;
public:
    void SetUniformMaterial(Material);
    void SetScreenWidthHeight(float width, float height);
    void SetTextureEnabled(Texture *, bool);
    void SetResamplingMode(ResamplingModes);

    virtual void ShadeFragmentRelativeToCamera(Vertex &, Camera &, Buffer *buffers){};
};
