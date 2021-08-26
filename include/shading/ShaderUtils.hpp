#pragma once

#include <GL3/gl3.h>
#include <GL3/gl3w.h>

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

#define VERTEX_SHADER_PATH "../src/shading/vertexShader.vert"
#define FRAGMENT_SHADER_PATH "../src/shading/fragmentShader.frag"

#define UNIFORM_DIFFUSE_COLOR_ID "uniformDiffuseColor"
#define UNIFORM_AMBIENT_COLOR_ID "uniformAmbientColor"
#define UNIFORM_SPECULAR_COLOR_ID "uniformSpecularColor"
#define UNIFORM_SHINE_COEFFICIENT_ID "uniformShineCoefficient"

#define UNIFORM_MODEL_ID "model"
#define UNIFORM_VIEW_ID "view"
#define UNIFORM_PROJECTION_ID "projection"

#define UNIFORM_LIGHTING_FUNCTION_ID "lightingShader"
#define LIGHTING_FUNCTION_FLAT "OpenGL_FlatShader"
#define LIGHTING_FUNCTION_GOURAUD_AD "OpenGL_AD_GouraudShader"
#define LIGHTING_FUNCTION_GOURAUD_ADS "OpenGL_ADS_GouraudShader"

#define UNIFORM_POSITION_FUNCTION_ID "positionShader"
#define POSITION_FUNCTION_OPENGL "OpenGL_VetexCoords"
#define POSITION_FUNCTION_CLOSE2GL "Close2GL_VertexCoords"

typedef struct
{
    GLenum type;
    const char *filename;
    GLuint shader;
} ShaderInfo;

void InitializeShaders();
static const GLchar *ReadShader(const char *filename);
GLuint LoadShaders(ShaderInfo *shaders);
GLuint GetShaderUniformVariableId(const char *uniformVariable);
void SetActiveVertexShaderSubroutine(const char *uniformFunction, const char *implementation);
