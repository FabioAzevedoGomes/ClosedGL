#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model3D.hpp"
#include "ShaderUtils.hpp"
#include "ModelViewProjectionUtils.hpp"
#include "Properties.hpp"
#include "colors.hpp"

#include <vector>

#define MAIN_WINDOW_WIDTH 800
#define MAIN_WINDOW_HEIGHT 700
#define ASPECT_RATIO MAIN_WINDOW_WIDTH / MAIN_WINDOW_HEIGHT

#define BUFFER_OFFSET(a) ((void *)(a))

enum Buffer_IDs
{
    VertexPositionBuffer,
    VertexNormalBuffer,
    VertexColorBuffer,
    NumBuffers
};
enum Attrib_IDs
{
    vertexPosition = 0,
    vertexColor = 1,
    vertexNormals = 2
};

enum VAO_IDss
{
    ModelObject,
    NumVAOs
};

void SetupVAOS(std::vector<Model3D> objects);
void SetupVBOS(std::vector<Model3D> objects);
void SetupShaders();
void BindObjectBuffers(Model3D object);

void DrawObject(Model3D object, Properties vertexProperties);
void RenderScene(std::vector<Model3D> objects, Camera &camera, Properties renderProperties);

void UpdateCameraValues(Camera &camera, Properties cameraProperties);
void ApplyFrameProperties(Properties frameProperties);
void ApplyVertexProperties(Properties vertexProperties);

void SetRenderMode(RenderModes renderMode);
void SetCullingMode(CullingModes cullingMode);
void SetRenderUniformColor(glm::vec3 color);

#endif
