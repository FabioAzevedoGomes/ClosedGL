#include "PassThroughLightingStrategy.hpp"

void PassThroughLightingStrategy::ShadeFragmentRelativeToCamera(Vertex &fragment, Camera &camera)
{
    // Only divide color to draw, as it is a pass-through strategy
    fragment.color /= fragment.wp;
}
