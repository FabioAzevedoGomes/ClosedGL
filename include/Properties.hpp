#ifndef PROPERTIES_H
#define PROPERTIES_H

enum RenderModes
{
    Standard,
    Wireframe
};

enum LightingModes
{
    Gouraud,
    Phong
};

typedef struct
{
    bool changed = true;
    RenderModes renderMode = Wireframe;
    LightingModes lightingMode = Gouraud;
} Properties;

#endif
