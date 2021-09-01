#include "OpenGLRenderer.hpp"

OpenGLRenderer::OpenGLRenderer()
{
    this->engineId = OpenGL;
    this->engineName = "OpenGL";
}

void OpenGLRenderer::DrawObject(Model3D object)
{
    // No model manipulation for now
    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(GetShaderUniformVariableId(UNIFORM_MODEL_ID), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glBindVertexArray(VAOs[ModelObject_OpenGL]);
    glDrawArrays(GL_TRIANGLES, 0, 3 * object.triangleCount);
    glBindVertexArray(0);
}

void OpenGLRenderer::RenderScene(Scene scene)
{
    glBindVertexArray(VAOs[ModelObject_OpenGL]);
    glUniformMatrix4fv(GetShaderUniformVariableId(UNIFORM_PROJECTION_ID), 1, GL_FALSE, glm::value_ptr(scene.camera.GetProjectionMatrix()));
    glUniformMatrix4fv(GetShaderUniformVariableId(UNIFORM_VIEW_ID), 1, GL_FALSE, glm::value_ptr(scene.camera.GetViewMatrix()));

    for (auto objectIterator = scene.models.begin(); objectIterator != scene.models.end(); objectIterator++)
        DrawObject(*objectIterator);
}

void OpenGLRenderer::BindObjectBuffers(Model3D object)
{
    // Position
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[OpenGL_VertexPositionBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(float), object.GetVertexPositionData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(openGLvertexPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(openGLvertexPosition);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[OpenGL_VertexColorBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(float), object.GetVertexColorData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(openGLvertexColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(openGLvertexColor);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[OpenGL_VertexNormalBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(float), object.GetVertexNormalData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(openGLvertexNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(openGLvertexNormals);
}

void OpenGLRenderer::SetupVAOS()
{
    glGenVertexArrays(NumVAOs_OpenGL, VAOs);
    glBindVertexArray(VAOs[ModelObject_OpenGL]);
}

void OpenGLRenderer::SetupVBOS(std::vector<Model3D> objects)
{
    glCreateBuffers(OpenGL_NumBuffers, Buffers);

    for (auto objectIterator = objects.begin(); objectIterator != objects.end(); objectIterator++)
        BindObjectBuffers(*objectIterator);
}

void OpenGLRenderer::SetCullingMode(CullingModes cullingMode)
{
    switch (cullingMode)
    {
    case BackfaceCulling:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        break;
    case FrontFaceCulling:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        break;
    case NoCulling:
        glDisable(GL_CULL_FACE);
    default:
        break;
    }
}

void OpenGLRenderer::SetPolygonOrientation(PolygonOrientation orientation)
{
    switch (orientation)
    {
    case CounterClockwise:
        glFrontFace(GL_CCW);
        break;
    case Clockwise:
    default:
        glFrontFace(GL_CW);
        break;
    }
}
