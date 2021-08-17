#include <GL3/gl3.h>
#include <GL3/gl3w.h>

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

#define VERTEX_SHADER_PATH "../src/shaders/vertexShader.vert"
#define FRAGMENT_SHADER_PATH "../src/shaders/fragmentShader.frag"

typedef struct
{
    GLenum type;
    const char *filename;
    GLuint shader;
} ShaderInfo;

GLuint InitializeShaders();
static const GLchar *ReadShader(const char *filename);
GLuint LoadShaders(ShaderInfo *shaders);
