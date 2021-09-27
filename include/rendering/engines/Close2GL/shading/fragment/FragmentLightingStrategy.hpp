#pragma once

#include "Camera.hpp"
#include "Vertex.hpp"
#include "Material.hpp"
#include "ShadingConstants.hpp"

class FragmentLightingStrategy
{
protected:
    Material uniformMaterial;
    float width, height;
public:
    void SetUniformMaterial(Material);
    void SetScreenWidthHeight(float width, float height);

    virtual void ShadeFragmentRelativeToCamera(Vertex &, Camera &){};
};
