#include "Color.h"
#include "DebugUI.h"
#include "Floor.h"
#include "GraphicsContext.h"
#include "Resources.h"
#include "GameApp.h"
#include "InputCodes.h"
#include "Monke.h"

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
};

void DemoApp::Startup() {
    auto demoScene = AScene::Create("Demo");
    auto floor     = IGameObject::Create<Floor>("Floor");
    auto monke     = IGameObject::Create<Monke>("Monke");
    auto mainCam   = IGameObject::Create<ACamera>("MainCamera");

    floor->GetTransform()->SetPosition(0.f, -1.f, 0.f);
    mainCam->SetActive(true);
    mainCam->GetTransform()->SetPosition(0.f, 0.f, -5.f);
    demoScene->GetSun().GetTransform().SetPosition(0.f, 0.f, -5.f);

    demoScene->AddGameObject(floor);
    demoScene->AddGameObject(monke);
    demoScene->AddGameObject(mainCam);

    AddScene(move(demoScene));
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
        default:
            break;
    }
}

int main(int, char* argv[]) {
    Resources::SetCwd(argv[0]);

    DemoApp app;
    Application::InitializeApp(app, SCREEN_792P, "XEN | DemoApp <OpenGL 4.6>");
    Utilities::SetWindowIcon(
      Resources::GetResource(RES_ROOT, "APP_ICON.png").c_str());
    Application::RunApp(app);

    return 0;
}
