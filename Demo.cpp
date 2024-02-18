#include "Color.h"
#include "DebugUI.h"
#include "GraphicsContext.h"
#include "Input.h"
#include "Resources.h"
#include "GameApp.h"
#include "Monke.h"

#include <fmt/format.h>

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

private:
    //====================================//
    // Put application-specific data here //
    //====================================//
    bool m_ShowDebugOutput = true;
};

void DemoApp::Startup() {
    // =============================== //
    //  Bind some default key actions  //
    // =============================== //
    Input::AddAction(GLFW_KEY_ESCAPE, GLFW_PRESS, [&]() {
        Graphics::MarkWindowForClose();
    });

    Input::AddAction(GLFW_KEY_TAB, GLFW_PRESS, [&]() {
        Graphics::ToggleWireframe();
    });

    Input::AddAction(GLFW_KEY_F12, GLFW_PRESS, [&]() {
        m_ShowDebugOutput = !m_ShowDebugOutput;
    });

    Input::AddAction(GLFW_KEY_F10, GLFW_PRESS, [&]() {
        Graphics::ToggleFullscreen();
    });

    Input::AddAction(GLFW_KEY_F11, GLFW_PRESS, [&]() {
        Graphics::ToggleVsync();
    });

    const auto demoScene = new AScene("Demo");
    const auto monke     = new Monke(0x00000002);
    const auto monke2    = new Monke(0x00000003);
    const auto monke3    = new Monke(0x00000004);
    const auto mainCam   = new ACamera;
    
    monke->GetTransform()->Scale(0.01, 0.01, 0.01);
    
    monke2->GetTransform()->Scale(0.01, 0.01, 0.01);
    monke2->GetTransform()->Translate(-3, 0, 3);
    
    monke3->GetTransform()->Scale(0.01, 0.01, 0.01);
    monke3->GetTransform()->Translate(3, 0, 3);
    
    mainCam->SetActive(true);
    mainCam->GetTransform()
      ->SetPositionAndRotation(0.f, 0.f, -5.f, 0.f, 0.f, 0.f);
      
    demoScene->AddGameObject(*monke);
    demoScene->AddGameObject(*monke2);
    demoScene->AddGameObject(*monke3);
    demoScene->AddGameObject(*mainCam);
    
    demoScene->GetContext().m_Sun.GetTransform().SetPositionAndRotation(0.f,
                                                                        0.f,
                                                                        -5.f,
                                                                        0.f,
                                                                        0.f,
                                                                        0.f);

    AddScene(*demoScene);
    LoadScene("Demo");
}

void DemoApp::Cleanup() {}

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
