#include "RenderUtils.hpp"

GLuint Buffers[NumBuffers];
GLuint VAOs[NumVAOs];

GLuint program;
GLuint uniformMvpId;
GLuint uniformColorId;
GLuint uniformCameraPosId;
GLuint uniformLightPosId;
GLuint uniformOrientationId;

GLuint uniformViewId;

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
    uniformCameraPosId = glGetUniformLocation(program, "cameraPosition");
    uniformLightPosId = glGetUniformLocation(program, "lightPosition");
    uniformOrientationId = glGetUniformLocation(program, "orientation");
    uniformViewId = glGetUniformLocation(program, "viewMatrix");

    glEnable(GL_DEPTH_TEST);
}

void BindObjectBuffers(Model3D object)
{
    // Position
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexPositionBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(float), object.GetVertexPositionData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vertexPosition);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexColorBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(float), object.GetVertexColorData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(vertexColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vertexColor);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexNormalBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 9 * sizeof(float), object.GetVertexNormalData(), GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(vertexNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vertexNormals);
}

void CalculateViewProjectionMatrix(Camera camera)
{
    glm::mat4 view = ModelViewProjectionUtils::GetViewMatrix(camera);
    glm::mat4 projection = ModelViewProjectionUtils::GetPerspectiveProjectionMatrix(camera, ASPECT_RATIO);

    glUniformMatrix4fv(uniformViewId, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

    projectionView = projection * view;
}

void DrawObject(Model3D object, Properties vertexProperties)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 modelViewProjection = projectionView * model;

    glUniformMatrix4fv(uniformMvpId, 1, GL_FALSE, glm::value_ptr(modelViewProjection));
    glBindVertexArray(VAOs[ModelObject]);
    glDrawArrays(GL_TRIANGLES, 0, 3 * object.triangleCount);
    glBindVertexArray(0);
}

void RenderScene(std::vector<Model3D> objects, Camera &camera, Properties properties)
{
    ApplyFrameProperties(properties);
    UpdateCameraValues(camera, properties);
    CalculateViewProjectionMatrix(camera);

    // TODO Move somewhere else
    glUniform4fv(uniformCameraPosId, 1, glm::value_ptr(glm::vec4(camera.position, 1.0f)));
    glUniform4fv(uniformLightPosId, 1, glm::value_ptr(glm::vec4(camera.position, 1.0f)));

    for (auto objectIterator = objects.begin(); objectIterator != objects.end(); objectIterator++)
        DrawObject(*objectIterator, properties);
}

void UpdateCameraValues(Camera &camera, Properties cameraProperties)
{
    if (!cameraProperties.keepLookingAtModel)
        camera.Rotate(cameraProperties.rotationPitch, cameraProperties.rotationRoll, cameraProperties.rotationYaw);
    else
        camera.LookAt();

    camera.nearPlane = cameraProperties.nearPlane;
    camera.farPlane = cameraProperties.farPlane;
    camera.fieldOfView = cameraProperties.fieldOfView;
}

void ApplyFrameProperties(Properties frameProperties)
{
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(frameProperties.backgroundColor, 0.0f)));
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    SetRenderMode(RenderModes(frameProperties.renderMode));
    SetRenderUniformColor(frameProperties.modelColor);
    SetCullingMode(CullingModes(frameProperties.cullingMode));
    SetPolygonOrientation(PolygonOrientation(frameProperties.orientation));
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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
        glCullFace(GL_NONE);
    default:
        break;
    }
}

void SetPolygonOrientation(PolygonOrientation orientation)
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
