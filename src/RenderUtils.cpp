#include "RenderUtils.hpp"

GLuint Buffers[NumBuffers];
GLuint VAOs[NumVAOs];

GLuint program;
GLuint uniformMvpId;
GLuint uniformColorId;

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
    uniformMvpId = glGetUniformLocation(program, "modelViewProjection");
    uniformColorId = glGetUniformLocation(program, "uniformColor");
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

void DrawObject(Model3D object, Properties vertexProperties)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 modelViewProjection = projectionView * model;

    ApplyVertexProperties(vertexProperties);

    glUniformMatrix4fv(uniformMvpId, 1, GL_FALSE, glm::value_ptr(modelViewProjection));
    glBindVertexArray(VAOs[ModelObject]);
    glDrawArrays(GL_TRIANGLES, 0, 3 * object.triangleCount);
    glBindVertexArray(0);
}

void RenderScene(std::vector<Model3D> objects, Camera &camera, Properties properties)
{
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(properties.backgroundColor, 0.0f)));

    ApplyFrameProperties(properties);
    UpdateCameraValues(camera, properties);

    view = ModelViewProjectionUtils::GetViewMatrix(camera);
    projection = ModelViewProjectionUtils::GetPerspectiveProjectionMatrix(camera, ASPECT_RATIO);
    projectionView = projection * view;

    for (auto objectIterator = objects.begin(); objectIterator != objects.end(); objectIterator++)
        DrawObject(*objectIterator, properties);
}

void UpdateCameraValues(Camera &camera, Properties cameraProperties)
{
    if (!cameraProperties.keepLookingAtModel)
        camera.Rotate(cameraProperties.rotationPitch, cameraProperties.rotationRoll, cameraProperties.rotationYaw);

    camera.nearPlane = cameraProperties.nearPlane;
    camera.farPlane = cameraProperties.farPlane;
    camera.fieldOfView = cameraProperties.fieldOfView;
}

void ApplyFrameProperties(Properties frameProperties)
{
    SetRenderMode(RenderModes(frameProperties.renderMode));
    SetRenderUniformColor(frameProperties.modelColor);
    SetCullingMode(CullingModes(frameProperties.cullingMode));
}

void ApplyVertexProperties(Properties vertexProperties)
{
}

void SetRenderMode(RenderModes renderMode)
{
    switch (renderMode)
    {
    case Wireframe:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case Points:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    case Standard:
    default:
        glPolygonMode(GL_FRONT, GL_FILL);
        break;
    }
}

void SetRenderUniformColor(glm::vec3 color)
{
    glUniform3fv(uniformColorId, 1, glm::value_ptr(color));
}

void SetCullingMode(CullingModes cullingMode)
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
