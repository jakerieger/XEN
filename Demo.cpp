#include "Color.h"
#include "GraphicsContext.h"
#include "Resources.h"
#include "GameApp.h"
#include "InputCodes.h"
#include "Monke.h"

#include "stb_image.h"

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
    bool m_ShowDebugOutput = true;
};

void DemoApp::Startup() {
    const auto demoScene = new AScene("Demo");
    const auto monke     = new Monke("monke1");
    const auto monke2    = new Monke("monke2");
    const auto monke3    = new Monke("monke3");
    const auto mainCam   = new ACamera("MainCamera");

    monke->GetTransform()->SetScale(0.01, 0.01, 0.01);

    monke2->GetTransform()->SetScale(0.01, 0.01, 0.01);
    monke2->GetTransform()->SetPosition(-3, 0, 3);

    monke3->GetTransform()->SetScale(0.01, 0.01, 0.01);
    monke3->GetTransform()->SetPosition(3, 0, 3);

    mainCam->SetActive(true);
    mainCam->GetTransform()->SetPosition(0.f, 0.f, -5.f);

    demoScene->AddGameObject(*monke);
    demoScene->AddGameObject(*monke2);
    demoScene->AddGameObject(*monke3);
    demoScene->AddGameObject(*mainCam);

    demoScene->GetSun().GetTransform().SetPosition(0.f, 0.f, -5.f);

    AddScene(*demoScene);
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
        case KeyCode::Tab:
            Graphics::ToggleWireframe();
            break;
        default:
            break;
    }
}

static void SetWindowIcon() {
    GLFWimage appIcon[1];
    appIcon[0].pixels =
      stbi_load(Resources::GetResource(RES_ROOT, "APP_ICON.png").c_str(),
                &appIcon[0].width,
                &appIcon[0].height,
                0,
                4);
    glfwSetWindowIcon(Graphics::GetWindow(), 1, appIcon);
    stbi_image_free(appIcon[0].pixels);
}

int main(int argc, char* argv[]) {
    Resources::SetCwd(argv[0]);

    DemoApp app;
    Application::InitializeApp(app,
                               SCREEN_792P,
                               "GLEngine | DemoApp <OpenGL 4.6>");
    SetWindowIcon();
    Application::RunApp(app);

    return 0;
}
