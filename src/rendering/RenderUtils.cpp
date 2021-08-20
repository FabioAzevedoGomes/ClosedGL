#include "RenderUtils.hpp"

GLuint Buffers[NumBuffers];
GLuint VAOs[NumVAOs];

GLuint program;
GLuint uniformDiffuseColorId;
GLuint uniformOrientationId;

GLuint uniformViewId;
GLuint uniformProjectionId;
GLuint uniformModelId;

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
    uniformDiffuseColorId = glGetUniformLocation(program, "uniformDiffuseColor");
    uniformOrientationId = glGetUniformLocation(program, "orientation");

    uniformModelId = glGetUniformLocation(program, "model");
    uniformProjectionId = glGetUniformLocation(program, "projection");
    uniformViewId = glGetUniformLocation(program, "view");

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
    glm::vec4 lookAtPoint = glm::vec4(camera.position, 1.0f) + glm::vec4(camera.n, 0.0f);
    glm::mat4 view = glm::lookAt(camera.position, glm::vec3(lookAtPoint), camera.v);

    glm::mat4 projection = glm::perspective(camera.fieldOfView, ASPECT_RATIO, camera.nearPlane, camera.farPlane);

    glUniformMatrix4fv(uniformProjectionId, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformViewId, 1, GL_FALSE, glm::value_ptr(view));
}

void DrawObject(Model3D object, Properties vertexProperties)
{
    // No model manipulation for now
    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glBindVertexArray(VAOs[ModelObject]);
    glDrawArrays(GL_TRIANGLES, 0, 3 * object.triangleCount);
    glBindVertexArray(0);
}

void RenderScene(std::vector<Model3D> objects, Camera &camera, Properties properties)
{
    ApplyFrameProperties(properties);
    UpdateCameraValues(camera, properties);
    CalculateViewProjectionMatrix(camera);

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
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(frameProperties.backgroundColor, 0.0f)));

    SetRenderMode(RenderModes(frameProperties.renderMode));
    SetRenderUniformColor(frameProperties.modelColor);
    SetCullingMode(CullingModes(frameProperties.cullingMode));
    SetPolygonOrientation(PolygonOrientation(frameProperties.orientation));
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
    glUniform3fv(uniformDiffuseColorId, 1, glm::value_ptr(color));
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

void SetPolygonOrientation(PolygonOrientation orientation)
{
    switch (orientation)
    {
    case CounterClockwise:
        glUniform1i(uniformOrientationId, 1);
        glFrontFace(GL_CCW);
        break;
    case Clockwise:
    default:
        glUniform1i(uniformOrientationId, -1);
        glFrontFace(GL_CW);
        break;
    }
}
