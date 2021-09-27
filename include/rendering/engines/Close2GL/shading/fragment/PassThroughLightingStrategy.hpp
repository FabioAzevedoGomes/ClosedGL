#pragma once

#include "FragmentLightingStrategy.hpp"

class PassThroughLightingStrategy : public FragmentLightingStrategy
{
    virtual void ShadeFragmentRelativeToCamera(Vertex &, Camera &);
};
