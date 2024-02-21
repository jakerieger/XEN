#include "Color.h"
#include "DebugUI.h"
#include "Floor.h"
#include "GraphicsContext.h"
#include "Resources.h"
#include "GameApp.h"
#include "InputCodes.h"
#include "SkyDome.h"
#include "TV.h"

#include <fmt/format.h>

const FSize SCREEN_720P  = {1280, 720};
const FSize SCREEN_792P  = {1408, 792};
const FSize SCREEN_900P  = {1600, 900};
const FSize SCREEN_1080P = {1920, 1080};

class DemoApp final : public IGameApp {
public:
    DemoApp() = default;
    void Startup() override;
    void Cleanup() override;
    void OnKeyDown(FKeyEvent& event) override;

private:
    //====================================//
    // Put application-specific data here //
    //====================================//
    bool m_HideCursor = false;
};

void DemoApp::Startup() {
    auto demoScene = AScene::Create("Demo");
    auto floor     = IGameObject::Create<Floor>("Floor");
    auto skydome   = IGameObject::Create<SkyDome>("SkyDome");
    auto mainCam   = IGameObject::Create<ACamera>("MainCamera");

    floor->GetTransform()->SetPosition(0.f, -1.f, 0.f);
    skydome->GetTransform()->SetScale(0.3f, 0.3f, 0.3f);

    mainCam->SetActive(true);
    mainCam->GetTransform()->SetPosition(0.f, 0.f, 5.f);

    demoScene->GetSun().GetTransform().SetPosition(2.f, 0.f, 5.f);
    demoScene->AddGameObject(floor);

    for (u8 i = 0; i < 10; i++) {
        for (u8 j = 0; j < 10; j++) {
            auto tv =
              IGameObject::Create<TV>(fmt::format("TV_{}.{}", i, j).c_str());
            tv->GetTransform()->SetPosition(i * 8, 0.f, j * 8);
            demoScene->AddGameObject(tv);
        }
    }

    demoScene->AddGameObject(skydome);
    demoScene->AddGameObject(mainCam);
    AddScene(demoScene);
    LoadScene("Demo");
}

void DemoApp::Cleanup() {}

void DemoApp::OnKeyDown(FKeyEvent& event) {
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

int main(int, char* argv[]) {
    Resources::SetCwd(argv[0]);

    DemoApp app;
    Application::InitializeApp(app, SCREEN_792P, "XEN | ModelLoading", true);
    Utilities::SetWindowIcon(
      Resources::GetResource(RES_ROOT, "APP_ICON.png").c_str());
    Application::RunApp(app);

    return 0;
}
