#include "Close2GLRenderer.hpp"

Close2GLRenderer::Close2GLRenderer()
{
    this->engineId = Close2GL;
    this->engineName = "Close2GL";

    colorBuffer = nullptr;
    depthBuffer = nullptr;
}

Close2GLRenderer::~Close2GLRenderer()
{
    free(colorBuffer);
    free(depthBuffer);
}

bool Close2GLRenderer::ShouldCull(std::vector<glm::vec4> triangleVertices)
{
    if (cullingMode == NoCulling)
        return false;

    float sum = 0.0f;
    for (int i = 0; i < triangleVertices.size(); i++)
    {
        float firstTerm = triangleVertices[i].x * triangleVertices[(i + 1) % triangleVertices.size()].y;
        float secondTerm = triangleVertices[(i + 1) % triangleVertices.size()].x * triangleVertices[i].y;

        sum += firstTerm - secondTerm;
    }

    float area = 0.5f * sum;
    return area * polygonOrientation * cullingMode > 0;
}

int Close2GLRenderer::PopulateVertexBuffer(Model3D object, float *vertexBuffer)
{
    glm::mat4 modelViewProjection = projection * view * model;
    int visibleVertexCount = 0;

    for (int triangle = 0; triangle < object.triangleCount; triangle++)
    {
        bool insideFrustum = false;

        Triangle modelTriangle = object.triangles[triangle];
        std::vector<glm::vec4> triangleVertices;
        for (int vertex = 0; vertex < 3; vertex++)
        {
            glm::vec4 vertexPosition = glm::vec4(modelTriangle.vertices[vertex].position, 1.0f);
            glm::vec4 projectedPosition = modelViewProjection * vertexPosition;

            // Clip triangle when w <= 0
            if (projectedPosition.w <= 0)
            {
                triangleVertices.clear();
                break;
            }

            // Perspective division
            projectedPosition /= projectedPosition.w;

            // Clip triangle before/after Near/Far planes
            if (projectedPosition.z < -1 || projectedPosition.z > 1)
            {
                triangleVertices.clear();
                break;
            }

            if (projectedPosition.x > -1 && projectedPosition.x < 1 && projectedPosition.y > -1 && projectedPosition.y < 1)
                insideFrustum = true;

            triangleVertices.push_back(projectedPosition);
        }

        if (insideFrustum && !ShouldCull(triangleVertices))
        {
            for (auto vertex = triangleVertices.begin(); vertex != triangleVertices.end(); vertex++)
            {
                vertexBuffer[(2 * visibleVertexCount) + 0] = (*vertex).x;
                vertexBuffer[(2 * visibleVertexCount) + 1] = (*vertex).y;
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
}

void Close2GLRenderer::CalculateRenderingMatrices(Scene scene, Window *window)
{
    view = scene.camera.GetViewMatrix();
    projection = scene.camera.GetProjectionMatrix();

    float n = scene.camera.nearPlane;
    float f = scene.camera.farPlane;
    float w = window->GetWidth();
    float h = window->GetHeight();

    viewport = glm::mat4(w / 2.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, h / 2.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, (f - n) / 2.0f, 0.0f,
                         w / 2.0f, h / 2.0f, (n + f) / 2.0f, 1.0f);
}

void Close2GLRenderer::ClearAndResizeBuffersForWindow(Window *window)
{
    free(colorBuffer);
    colorBuffer = (float ***)malloc(sizeof(float) * window->GetHeight() * window->GetWidth() * 4);
    memset(colorBuffer, 255, sizeof(float) * window->GetHeight() * window->GetWidth() * 4);

    free(depthBuffer);
    depthBuffer = (float **)malloc(sizeof(float) * window->GetHeight() * window->GetWidth());
    memset(depthBuffer, 0, sizeof(float) * window->GetHeight() * window->GetWidth());

    glDeleteTextures(1, Textures);
    glGenTextures(1, Textures);
    glBindTexture(GL_TEXTURE_2D, Textures[ModelTexture_Close2GL]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, window->GetWidth(), window->GetHeight());
}

void Close2GLRenderer::RenderSceneToWindow(Scene scene, Window *window)
{
    CalculateRenderingMatrices(scene, window);
    ClearAndResizeBuffersForWindow(window);

    for (auto object = scene.models.begin(); object != scene.models.end(); object++)
    {
        model = glm::mat4(1.0f);
        DrawObject(*object);
    }

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, Textures[ModelTexture_Close2GL]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, window->GetWidth(),
                    window->GetHeight(), GL_RGBA, GL_FLOAT,
                    colorBuffer);

    glBindVertexArray(VAOs[ModelObject_Close2GL]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
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

    float vertexData[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
                          1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f};
    float textureCoordinateData[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                                     1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f};

    // Position
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[Close2GL_VertexPositionBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), vertexData, GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(close2GLvertexPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(close2GLvertexPosition);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[Close2GL_VertexPositionBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), textureCoordinateData, GL_DYNAMIC_STORAGE_BIT);
    glVertexAttribPointer(close2GLtextureCoordinates, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(close2GLtextureCoordinates);

    // Textures
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, Textures);
    glBindTexture(GL_TEXTURE_2D, Textures[ModelTexture_Close2GL]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, 0, 0);
}

void Close2GLRenderer::SetCullingMode(CullingModes cullingMode)
{
    glDisable(GL_CULL_FACE);
    this->cullingMode = cullingMode;
}

void Close2GLRenderer::SetPolygonOrientation(PolygonOrientation orientation)
{
    this->polygonOrientation = orientation;
}
