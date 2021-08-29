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
        SetActiveVertexShaderSubroutine(UNIFORM_LIGHTING_FUNCTION_ID, LIGHTING_FUNCTION_FLAT);
    else
        switch (lightingMode)
        {
        case Flat:
            SetActiveVertexShaderSubroutine(UNIFORM_LIGHTING_FUNCTION_ID, LIGHTING_FUNCTION_FLAT);
            break;
        case Gouraud_AD:
            SetActiveVertexShaderSubroutine(UNIFORM_LIGHTING_FUNCTION_ID, LIGHTING_FUNCTION_GOURAUD_AD);
            break;
        case Gouraud_ADS:
            SetActiveVertexShaderSubroutine(UNIFORM_LIGHTING_FUNCTION_ID, LIGHTING_FUNCTION_GOURAUD_ADS);
            break;
        default:
            break;
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

void RenderingManager::SelectRenderUniformColor(glm::vec3 diffuseColor, glm::vec3 ambientColor, glm::vec3 specularColor, float shineCoefficient)
{
    glUniform3fv(GetShaderUniformVariableId(UNIFORM_DIFFUSE_COLOR_ID), 1, glm::value_ptr(diffuseColor));
    glUniform3fv(GetShaderUniformVariableId(UNIFORM_AMBIENT_COLOR_ID), 1, glm::value_ptr(ambientColor));
    glUniform3fv(GetShaderUniformVariableId(UNIFORM_SPECULAR_COLOR_ID), 1, glm::value_ptr(specularColor));
    glUniform1f(GetShaderUniformVariableId(UNIFORM_SHINE_COEFFICIENT_ID), shineCoefficient);
}

void RenderingManager::SelectCullingMode(CullingModes cullingMode)
{
    // Disable for close2GL because we do the clipping ourselves
    if (selectedRenderingEngine->engineId == Close2GL)
        glDisable(GL_CULL_FACE);
    else
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

void RenderingManager::SelectPolygonOrientation(PolygonOrientation orientation)
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
