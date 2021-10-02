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

    if (object.texture != nullptr && object.texture->exists() && object.texture->needsReload)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, object.texture->width, object.texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, object.texture->image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glUniformMatrix4fv(GetShaderUniformVariableId(UNIFORM_MODEL_ID), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glBindVertexArray(VAOs[ModelObject_OpenGL]);
    glDrawArrays(GL_TRIANGLES, 0, 3 * object.triangleCount);
    glBindVertexArray(0);
}

void OpenGLRenderer::RenderSceneToWindow(Scene scene, Window *_)
{
    glBindVertexArray(VAOs[ModelObject_OpenGL]);
    glUniformMatrix4fv(GetShaderUniformVariableId(UNIFORM_PROJECTION_ID), 1, GL_FALSE, glm::value_ptr(scene.camera.GetProjectionMatrix()));
    glUniformMatrix4fv(GetShaderUniformVariableId(UNIFORM_VIEW_ID), 1, GL_FALSE, glm::value_ptr(scene.camera.GetViewMatrix()));
    glUniform1i(GetShaderUniformVariableId(UNIFORM_OPENGL_TEXTURE_ENABLED_ID), state.textureOn);

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

    if (object.textured)
    {
        // Textures
        glBindBuffer(GL_ARRAY_BUFFER, Buffers[OpenGL_VertexTexCoordBuffer]);
        glBufferStorage(GL_ARRAY_BUFFER, object.triangleCount * 6 * sizeof(float), object.GetTextureCoordinateData(), GL_DYNAMIC_STORAGE_BIT);
        glVertexAttribPointer(openGLtextureCoordinates, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(openGLtextureCoordinates);

        glGenTextures(1, Textures);
        glBindTexture(GL_TEXTURE_2D, Textures[ModelTexture_OpenGL]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        if (object.texture != nullptr && object.texture->exists() && object.texture->needsReload)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, object.texture->width, object.texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, object.texture->image);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
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

// State setters

void OpenGLRenderer::SetState(State state)
{
    this->state = state;

    SetCullingMode(state.cullingMode);
    SetPolygonOrientation(state.polygonOrientation);
    SetRenderMode(state.renderMode);
    SetLightingAlgorithm(state.lightingMode);
    SetRenderUniformMaterial(state.uniformMaterial);
    SetResamplingMode(state.resamplingMode);
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

void OpenGLRenderer::SetRenderMode(RenderModes renderMode)
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

void OpenGLRenderer::SetLightingAlgorithm(LightingModes lightingMode)
{
    switch (lightingMode)
    {
    case Flat:
        SetActiveVertexShaderSubroutine(UNIFORM_VSHADER_LIGHTING_FUNCTION_ID, VSHADER_LIGHTING_FUNCTION_FLAT);
        SetActiveFragmentShaderSubroutine(UNIFORM_FSHADER_LIGHTING_FUNCTION_ID, FSHADER_LIGHTING_FUNCTION_PASS_THROUGH);
        break;
    case Gouraud_AD:
        SetActiveVertexShaderSubroutine(UNIFORM_VSHADER_LIGHTING_FUNCTION_ID, VSHADER_LIGHTING_FUNCTION_GOURAUD_AD);
        SetActiveFragmentShaderSubroutine(UNIFORM_FSHADER_LIGHTING_FUNCTION_ID, FSHADER_LIGHTING_FUNCTION_PASS_THROUGH);
        break;
    case Gouraud_ADS:
        SetActiveVertexShaderSubroutine(UNIFORM_VSHADER_LIGHTING_FUNCTION_ID, VSHADER_LIGHTING_FUNCTION_GOURAUD_ADS);
        SetActiveFragmentShaderSubroutine(UNIFORM_FSHADER_LIGHTING_FUNCTION_ID, FSHADER_LIGHTING_FUNCTION_PASS_THROUGH);
        break;
    case Phong:
        SetActiveVertexShaderSubroutine(UNIFORM_VSHADER_LIGHTING_FUNCTION_ID, VSHADER_LIGHTING_FUNCTION_PHONG);
        SetActiveFragmentShaderSubroutine(UNIFORM_FSHADER_LIGHTING_FUNCTION_ID, FSHADER_LIGHTING_FUNCTION_PHONG);
        break;
    default:
        break;
    }
}

void OpenGLRenderer::SetRenderUniformMaterial(Material material)
{
    glUniform3fv(GetShaderUniformVariableId(UNIFORM_DIFFUSE_COLOR_ID), 1, glm::value_ptr(material.diffuseColor));
    glUniform1f(GetShaderUniformVariableId(UNIFORM_DIFFUSE_INTENSITY_ID), material.diffuseIntensity);
    glUniform3fv(GetShaderUniformVariableId(UNIFORM_AMBIENT_COLOR_ID), 1, glm::value_ptr(material.ambientColor));
    glUniform1f(GetShaderUniformVariableId(UNIFORM_AMBIENT_INTENSITY_ID), material.ambientIntensity);
    glUniform3fv(GetShaderUniformVariableId(UNIFORM_SPECULAR_COLOR_ID), 1, glm::value_ptr(material.specularColor));
    glUniform1f(GetShaderUniformVariableId(UNIFORM_SPECULAR_INTENSITY_ID), material.specularIntensity);
    glUniform1f(GetShaderUniformVariableId(UNIFORM_SHINE_COEFFICIENT_ID), material.shineCoefficient);
}

void OpenGLRenderer::SetResamplingMode(ResamplingModes resamplingMode)
{
    switch (resamplingMode)
    {
    case Trilinear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        break;
    case Bilinear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case NearestNeighbor:
    default:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    }
}
