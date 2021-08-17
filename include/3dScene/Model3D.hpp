#ifndef MODEL3D_H
#define MODEL3D_H

#include <glm/vec3.hpp>

#include "Triangle.hpp"
#include "Material.hpp"
#include "BoundingBox.hpp"

#include <cstdlib>
#include <iostream>

class Model3D
{
public:
    std::string fileName;
    char *modelName;

    Triangle *triangles;
    int triangleCount;

    Material *materials;
    int materialCount;

    BoundingBox boundingBox;

    Model3D();
    Model3D(std::string filename);
    float *GetVertexPositionData();
    float *GetVertexNormalData();
    float *GetVertexColorData();
    void PrintInformation();
    void SetMaterialDiffuseColor(int material, glm::vec3 color);

private:
    void ReadMaterial(FILE *file, int index);
    void ReadTriangle(FILE *file, int index);
};

#endif
