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

void RenderingManager::RenderSceneToWindow(Scene scene, Window *window)
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(selectedRenderingEngine->state.backgrounColor, 0.0f)));

    selectedRenderingEngine->RenderSceneToWindow(scene, window);
}

std::string RenderingManager::GetActiveEngineName()
{
    return selectedRenderingEngine->engineName;
}

void RenderingManager::SelectEngine(Engines renderingEngine, Scene scene)
{
    Engines previous = selectedRenderingEngine->engineId;
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

    if (previous != renderingEngine)
        SetupBuffers(scene);
}

void RenderingManager::SetEngineState(State state)
{
    selectedRenderingEngine->SetState(state);
}
