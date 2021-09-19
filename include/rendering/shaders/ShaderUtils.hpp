#pragma once

#include <GL3/gl3.h>
#include <GL3/gl3w.h>

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

#define VERTEX_SHADER_PATH "../src/rendering/shaders/vertexShader.vert"
#define FRAGMENT_SHADER_PATH "../src/rendering/shaders/fragmentShader.frag"

#define UNIFORM_DIFFUSE_COLOR_ID "uniformDiffuseColor"
#define UNIFORM_DIFFUSE_INTENSITY_ID "uniformDiffuseIntensity"
#define UNIFORM_AMBIENT_COLOR_ID "uniformAmbientColor"
#define UNIFORM_AMBIENT_INTENSITY_ID "uniformAmbientIntensity"
#define UNIFORM_SPECULAR_COLOR_ID "uniformSpecularColor"
#define UNIFORM_SPECULAR_INTENSITY_ID "uniformSpecularIntensity"
#define UNIFORM_SHINE_COEFFICIENT_ID "uniformShineCoefficient"

#define UNIFORM_MODEL_ID "model"
#define UNIFORM_VIEW_ID "view"
#define UNIFORM_PROJECTION_ID "projection"

#define UNIFORM_VSHADER_LIGHTING_FUNCTION_ID "vertexLightingShader"
#define VSHADER_LIGHTING_FUNCTION_FLAT "OpenGL_FlatShader"
#define VSHADER_LIGHTING_FUNCTION_GOURAUD_AD "OpenGL_AD_GouraudShader"
#define VSHADER_LIGHTING_FUNCTION_GOURAUD_ADS "OpenGL_ADS_GouraudShader"
#define VSHADER_LIGHTING_FUNCTION_PHONG "OpenGL_PhongShader"
#define VSHADER_LIGHTING_FUNCTION_TEXTURE "Close2GL_TextureShader"

#define UNIFORM_FSHADER_LIGHTING_FUNCTION_ID "fragmentLightingShader"
#define FSHADER_LIGHTING_FUNCTION_PHONG "OpenGL_PhongShader"
#define FSHADER_LIGHTING_FUNCTION_PASS_THROUGH "OpenGL_PassThroughShader"
#define FSHADER_LIGHTING_FUNCTION_TEXTURE "Close2GL_TextureShader"

#define UNIFORM_POSITION_FUNCTION_ID "vertexPositionShader"
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
void SetActiveFragmentShaderSubroutine(const char *uniformFunction, const char *implementation);
