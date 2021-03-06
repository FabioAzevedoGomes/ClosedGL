#ifndef MODEL3D_H
#define MODEL3D_H

#include <glm/vec3.hpp>

#include "Triangle.hpp"
#include "Material.hpp"
#include "BoundingBox.hpp"
#include "Texture.hpp"

#include <cstdlib>
#include <iostream>

#define MAX_NAME_LENGTH 100

class Model3D
{
public:
    std::string fileName;
    char modelName[MAX_NAME_LENGTH];

    Triangle *triangles;
    int triangleCount;

    Material *materials;
    int materialCount;

    BoundingBox boundingBox;

    Texture *texture;
    bool textured;

    Model3D();
    Model3D(std::string filename);
    float *GetVertexPositionData();
    float *GetVertexNormalData();
    float *GetVertexColorData();
    float *GetTextureCoordinateData();
    void PrintInformation();

private:
    void ReadMaterial(FILE *file, int index);
    void ReadTriangle(FILE *file, int index);
};

#endif
