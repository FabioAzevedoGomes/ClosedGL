#include "RenderingManager.hpp"

RenderingManager::RenderingManager()
{
    selectedRenderingEngine = &openGLRenderer;
}

void RenderingManager::SetupBuffers(Scene scene)
{
    selectedRenderingEngine->SetupVAOS();
    selectedRenderingEngine->SetupVBOS(scene.models);
}

void RenderingManager::RenderScene(Scene scene)
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(selectedRenderingEngine->backgroundColor, 0.0f)));

    selectedRenderingEngine->RenderScene(scene);
}

std::string RenderingManager::GetActiveEngineName()
{
    return selectedRenderingEngine->engineName;
}

void RenderingManager::SelectEngine(Engines renderingEngine, Scene scene)
{
    Renderer *previous = &(*selectedRenderingEngine);
    switch (renderingEngine)
    {
    case OpenGL:
        selectedRenderingEngine = &openGLRenderer;
        SetActiveVertexShaderSubroutine(UNIFORM_POSITION_FUNCTION_ID, POSITION_FUNCTION_OPENGL);
        break;
    case Close2GL:
        selectedRenderingEngine = &close2GLRenderer;
        SetActiveVertexShaderSubroutine(UNIFORM_POSITION_FUNCTION_ID, POSITION_FUNCTION_CLOSE2GL);
        break;
    }

    if (previous != selectedRenderingEngine)
        SetupBuffers(scene);
}

void RenderingManager::SelectLightingAlgorithm(LightingModes lightingMode)
{
    // Override lighting for now when using Close2GL
    if (selectedRenderingEngine->engineId == Close2GL)
    {
        SetActiveVertexShaderSubroutine(UNIFORM_VSHADER_LIGHTING_FUNCTION_ID, VSHADER_LIGHTING_FUNCTION_FLAT);
        SetActiveFragmentShaderSubroutine(UNIFORM_FSHADER_LIGHTING_FUNCTION_ID, FSHADER_LIGHTING_FUNCTION_PASS_THROUGH);
    }
    else
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
}

void RenderingManager::SelectRenderMode(RenderModes renderMode)
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

void RenderingManager::SelectBackgroundColor(glm::vec3 color)
{
    // Change for both
    openGLRenderer.backgroundColor = color;
    close2GLRenderer.backgroundColor = color;
}

void RenderingManager::SelectRenderUniformColor(glm::vec3 diffuseColor, float diffuseIntensity, glm::vec3 ambientColor, float ambientIntensity, glm::vec3 specularColor, float specularIntensity, float shineCoefficient)
{
    glUniform3fv(GetShaderUniformVariableId(UNIFORM_DIFFUSE_COLOR_ID), 1, glm::value_ptr(diffuseColor));
    glUniform1f(GetShaderUniformVariableId(UNIFORM_DIFFUSE_INTENSITY_ID), diffuseIntensity);
    glUniform3fv(GetShaderUniformVariableId(UNIFORM_AMBIENT_COLOR_ID), 1, glm::value_ptr(ambientColor));
    glUniform1f(GetShaderUniformVariableId(UNIFORM_AMBIENT_INTENSITY_ID), ambientIntensity);
    glUniform3fv(GetShaderUniformVariableId(UNIFORM_SPECULAR_COLOR_ID), 1, glm::value_ptr(specularColor));
    glUniform1f(GetShaderUniformVariableId(UNIFORM_SPECULAR_INTENSITY_ID), specularIntensity);

    glUniform1f(GetShaderUniformVariableId(UNIFORM_SHINE_COEFFICIENT_ID), shineCoefficient);
}

void RenderingManager::SelectCullingMode(CullingModes cullingMode)
{
    selectedRenderingEngine->SetCullingMode(cullingMode);
}

void RenderingManager::SelectPolygonOrientation(PolygonOrientation orientation)
{
    selectedRenderingEngine->SetPolygonOrientation(orientation);
}
