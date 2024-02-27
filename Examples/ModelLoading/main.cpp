#include "Engine/DebugUI.h"
#include "Engine/GraphicsContext.h"
#include "Engine/Resources.h"
#include "Engine/GameApp.h"
#include "Color.h"
#include "Floor.h"
#include "InputCodes.h"
#include "Statue.h"

const FSize SCREEN_720P  = {1280, 720};
const FSize SCREEN_792P  = {1408, 792};
const FSize SCREEN_900P  = {1600, 900};
const FSize SCREEN_1080P = {1920, 1080};

class ModelLoading final : public IGameApp {
public:
    ModelLoading() = default;
    void Startup() override;
    void Cleanup() override;
    void OnKeyDown(FKeyEvent& event) override;

private:
    //====================================//
    // Put application-specific data here //
    //====================================//
    bool m_HideCursor = true;
};

void ModelLoading::Startup() {
    using namespace eastl;

    auto demoScene = AScene::Create("Demo");
    auto mainCam   = IGameObject::Create<ACamera>("MainCamera");
    auto floor     = IGameObject::Create<Floor>("Floor");
    auto sun       = ADirectionalLight::Create({0.f, 1.f, 3.f});
    auto statue    = IGameObject::Create<Statue>("Statue");

    floor->GetTransform()->SetPosition(0.f, -1.f, 0.f);

    mainCam->SetActive(true);
    mainCam->GetTransform()->SetPosition(0.f, 0.f, 3.f);

    demoScene->SetSun(sun);
    demoScene->AddGameObject(floor);
    demoScene->AddGameObject(statue);
    demoScene->AddGameObject(mainCam);

    AddScene(demoScene);
    LoadScene("Demo");
}

void ModelLoading::Cleanup() {}

void ModelLoading::OnKeyDown(FKeyEvent& event) {
    IInputListener::OnKeyDown(event);

    switch (event.KeyCode) {
        case KeyCode::Escape:
            Graphics::MarkWindowForClose();
            break;
        case KeyCode::F10:
            Graphics::ToggleFullscreen();
            break;
        case KeyCode::F11:
            Graphics::ToggleVsync();
            break;
        case KeyCode::F12:
            DebugUI::ToggleVisible();
            break;
        case KeyCode::Tab:
            Graphics::ToggleWireframe();
            break;
        case KeyCode::Space: {
            if (m_HideCursor) {
                glfwSetInputMode(Graphics::GetWindow(),
                                 GLFW_CURSOR,
                                 GLFW_CURSOR_NORMAL);
            } else {
                glfwSetInputMode(Graphics::GetWindow(),
                                 GLFW_CURSOR,
                                 GLFW_CURSOR_DISABLED);
            }

            m_HideCursor = !m_HideCursor;
        } break;
        default:
            break;
    }
}

CREATE_AND_RUN(ModelLoading, SCREEN_792P, false)