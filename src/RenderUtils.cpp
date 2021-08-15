#include "RenderUtils.hpp"

GLuint Buffers[NumBuffers];
GLuint VAOs[NumVAOs];

GLuint program;
GLuint uniformViewId;
GLuint uniformModelId;
GLuint uniformProjectionId;

glm::mat4 view;
glm::mat4 projection;
glm::mat4 projectionView;

void SetupVAOS(std::vector<Model3D> objects)
{
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[ModelObject]);
}

void SetupVBOS(std::vector<Model3D> objects)
{
    glCreateBuffers(NumBuffers, Buffers);

    for (auto objectIterator = objects.begin(); objectIterator != objects.end(); objectIterator++)
        BindObjectBuffers(*objectIterator);
}

void SetupShaders()
{
    program = InitializeShaders();
    uniformViewId = glGetUniformLocation(program, "view");
    uniformProjectionId = glGetUniformLocation(program, "projection");
    uniformModelId = glGetUniformLocation(program, "model");
}

void BindObjectBuffers(Model3D object)
{
    // Position
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexPositionBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(GL_FLOAT), object.GetVertexPositionData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vertexPosition);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexColorBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(GL_FLOAT), object.GetVertexColorData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(vertexColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vertexColor);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexNormalBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(GL_FLOAT), object.GetVertexNormalData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(vertexNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vertexNormals);
}

void DrawObject(Model3D object)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 modelViewProjection = projectionView * model;

    glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjection"), 1, GL_FALSE, glm::value_ptr(modelViewProjection));
    glBindVertexArray(VAOs[ModelObject]);
    glDrawArrays(GL_TRIANGLES, 0, 3 * object.triangleCount);
    glBindVertexArray(0);
}

void SetRenderMode(RenderModes renderMode)
{
    switch (renderMode)
    {
    case Wireframe:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case Standard:
    default:
        glPolygonMode(GL_FRONT, GL_FILL);
        break;
    }
}

void RenderScene(std::vector<Model3D> objects, Camera camera)
{
    glClearBufferfv(GL_COLOR, 0, BLACK);

    view = ModelViewProjectionUtils::GetViewMatrix(camera);
    projection = ModelViewProjectionUtils::GetPerspectiveProjectionMatrix(camera, ASPECT_RATIO);
    projectionView = projection * view;

    for (auto objectIterator = objects.begin(); objectIterator != objects.end(); objectIterator++)
        DrawObject(*objectIterator);
}
