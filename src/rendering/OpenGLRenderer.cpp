#include "OpenGLRenderer.hpp"

OpenGLRenderer::OpenGLRenderer()
{
}

void OpenGLRenderer::DrawObject(Model3D object)
{
    // No model manipulation for now
    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(GetShaderUniformVariableId(UNIFORM_MODEL_ID), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glBindVertexArray(VAOs[ModelObject]);
    glDrawArrays(GL_TRIANGLES, 0, 3 * object.triangleCount);
    glBindVertexArray(0);
}

void OpenGLRenderer::RenderScene(Scene scene)
{
    glUniformMatrix4fv(GetShaderUniformVariableId(UNIFORM_PROJECTION_ID), 1, GL_FALSE, glm::value_ptr(scene.camera.GetProjectionMatrix()));
    glUniformMatrix4fv(GetShaderUniformVariableId(UNIFORM_VIEW_ID), 1, GL_FALSE, glm::value_ptr(scene.camera.GetViewMatrix()));

    for (auto objectIterator = scene.models.begin(); objectIterator != scene.models.end(); objectIterator++)
        DrawObject(*objectIterator);
}

void OpenGLRenderer::BindObjectBuffers(Model3D object)
{
    // Position
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexPositionBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(float), object.GetVertexPositionData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(openGLvertexPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(openGLvertexPosition);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexColorBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(float), object.GetVertexColorData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(openGLvertexColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(openGLvertexColor);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexNormalBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(float), object.GetVertexNormalData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(openGLvertexNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(openGLvertexNormals);
}

void OpenGLRenderer::SetupVAOS(std::vector<Model3D> objects)
{
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[ModelObject]);
}

void OpenGLRenderer::SetupVBOS(std::vector<Model3D> objects)
{
    glCreateBuffers(NumBuffers, Buffers);

    for (auto objectIterator = objects.begin(); objectIterator != objects.end(); objectIterator++)
        BindObjectBuffers(*objectIterator);
}
