#include "Color.h"
#include "DebugUI.h"
#include "Floor.h"
#include "GraphicsContext.h"
#include "Resources.h"
#include "GameApp.h"
#include "InputCodes.h"
#include "Statue.h"

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
    bool m_HideCursor = true;
};

void DemoApp::Startup() {
    using namespace eastl;

    unique_ptr<AScene> demoScene = AScene::Create("Demo");
    unique_ptr<ACamera> mainCam  = IGameObject::Create<ACamera>("MainCamera");
    unique_ptr<Floor> floor      = IGameObject::Create<Floor>("Floor");
    unique_ptr<Statue> statue    = IGameObject::Create<Statue>("Statue");

    unique_ptr<ADirectionalLight> sun =
      ADirectionalLight::Create({0.f, 2.f, 3.f});

    floor->GetTransform()->SetPosition(0.f, -1.f, 0.f);

    mainCam->SetActive(true);
    mainCam->GetTransform()->SetPosition(0.f, 0.f, 3.f);

    demoScene->SetSun(sun);
    demoScene->AddGameObject(floor);
    demoScene->AddGameObject(statue);
    demoScene->AddGameObject(mainCam);

    AddScene(demoScene);

    unique_ptr<AScene> demoScene2 = AScene::Create("Demo2");
    unique_ptr<ACamera> mainCam2  = IGameObject::Create<ACamera>("MainCamera2");
    unique_ptr<Floor> floor2      = IGameObject::Create<Floor>("Floor2");
    unique_ptr<Statue> statue2    = IGameObject::Create<Statue>("Statue2");

    unique_ptr<ADirectionalLight> sun2 =
      ADirectionalLight::Create({0.f, 4.f, 3.f});

    floor2->GetTransform()->SetPosition(0.f, -1.f, 0.f);

    mainCam2->SetActive(true);
    mainCam2->GetTransform()->SetPosition(0.f, 0.f, 10.f);

    demoScene2->SetSun(sun2);
    demoScene2->AddGameObject(floor2);
    demoScene2->AddGameObject(statue2);
    demoScene2->AddGameObject(mainCam2);

    AddScene(demoScene2);
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
        case KeyCode::Enter:
            LoadScene("Demo2");
            break;
        default:
            break;
    }
}

int main(int, char* argv[]) {
    Resources::SetCwd(argv[0]);

    DemoApp app;
    Application::InitializeApp(app, SCREEN_792P, "XEN | ModelLoading", false);
    Utilities::SetWindowIcon(
      Resources::GetResource(RES_ROOT, "APP_ICON.png").c_str());
    Application::RunApp(app);

    return 0;
}
