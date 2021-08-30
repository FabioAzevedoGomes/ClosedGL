#include "Close2GLRenderer.hpp"

#define DEBUG false

Close2GLRenderer::Close2GLRenderer()
{
    this->engineId = Close2GL;
    this->engineName = "Close2GL";
}

int Close2GLRenderer::PopulateVertexBuffer(Model3D object, float *vertexBuffer)
{
    glm::mat4 modelViewProjection = projection * view * model;
    int visibleVertexCount = 0;

    for (int triangle = 0; triangle < object.triangleCount; triangle++)
    {
        for (int vertex = 0; vertex < 3; vertex++)
        {
            // Get projected vertex position
            glm::vec4 vertexPosition = glm::vec4(object.triangles[triangle].vertices[vertex].position, 1.0f);
            glm::vec4 projectedPosition = modelViewProjection * vertexPosition;

            // If final position is outside of frustum
            if (projectedPosition.w <= 0)
            {
                // Remove the triangle this vertex belongs to, and skip to the next
                visibleVertexCount -= vertex;
                vertex = 4;
            }
            else
            {
                projectedPosition /= projectedPosition.w;
                vertexBuffer[(2 * visibleVertexCount) + 0] = projectedPosition.x;
                vertexBuffer[(2 * visibleVertexCount) + 1] = projectedPosition.y;
                visibleVertexCount++;
            }
        }
    }

    return visibleVertexCount;
}

void Close2GLRenderer::DrawObject(Model3D object)
{
    float *vertexBuffer = (float *)malloc(sizeof(float) * object.triangleCount * 6);

    int visibleVertexCount = PopulateVertexBuffer(object, vertexBuffer);

    glBindVertexArray(VAOs[ModelObject_Close2GL]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, visibleVertexCount * 2 * sizeof(float), vertexBuffer);
    glDrawArrays(GL_TRIANGLES, 0, visibleVertexCount);
    glBindVertexArray(0);
}

void Close2GLRenderer::RenderScene(Scene scene)
{
    view = scene.camera.GetViewMatrix();
    projection = scene.camera.GetProjectionMatrix();
    viewport = scene.camera.GetViewPortMatrix();

    for (auto object = scene.models.begin(); object != scene.models.end(); object++)
    {
        model = glm::mat4(1.0f);
        DrawObject(*object);
    }
}

void Close2GLRenderer::BindObjectBuffers(Model3D object)
{
    // Position
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[Close2GL_VertexPositionBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 6 * sizeof(float), object.GetVertexPositionData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(close2GLvertexPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(close2GLvertexPosition);
}

void Close2GLRenderer::SetupVAOS()
{
    glGenVertexArrays(NumVAOs_Close2GL, VAOs);
    glBindVertexArray(VAOs[ModelObject_Close2GL]);
}

void Close2GLRenderer::SetupVBOS(std::vector<Model3D> objects)
{
    glCreateBuffers(Close2GL_NumBuffers, Buffers);

    for (auto objectIterator = objects.begin(); objectIterator != objects.end(); objectIterator++)
        BindObjectBuffers(*objectIterator);
}
