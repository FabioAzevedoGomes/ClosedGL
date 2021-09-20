#pragma once

#include "Camera.hpp"
#include "Vertex.hpp"
#include "ShadingConstants.hpp"

class FragmentLightingStrategy
{
public:
    virtual void ShadeFragmentRelativeToCamera(Vertex &, Camera &){};
};
