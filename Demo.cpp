#include "Camera.h"
#include "Color.h"
#include "DebugUI.h"
#include "GraphicsContext.h"
#include "Input.h"
#include "Resources.h"
#include "GameApp.h"
#include "Model.h"
#include "PostProcessing.h"
#include "Profiler.h"
#include "Scene.h"
#include "Shader.h"
#include <fmt/format.h>

#include "stb_image.h"

#define SCREEN_720P                                                            \
    FSize {                                                                    \
        1280, 720                                                              \
    }
#define SCREEN_792P                                                            \
    FSize {                                                                    \
        1408, 792                                                              \
    }
#define SCREEN_900P                                                            \
    FSize {                                                                    \
        1600, 900                                                              \
    }
#define SCREEN_1080P                                                           \
    FSize {                                                                    \
        1920, 1080                                                             \
    }

class DemoApp final : public IGameApp {
public:
    DemoApp() = default;
    void Startup() override;
    void Cleanup() override;
    bool IsDone() override;
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void RenderScene() override;
    void RenderUI() override;

private:
    //====================================//
    // Put application-specific data here //
    //====================================//
    bool m_ShowDebugOutput = true;
    AScene* m_Scene        = nullptr;
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

    // ========================= //
    //  Initialize demo content  //
    // ========================= //
    m_Scene = new AScene();
    m_Scene->Initialize();

    ACamera camera(glm::vec3(0.f, 0.f, -5.f));
    camera.SetActive(true);
    m_Scene->AddCamera(std::move(camera));

    auto gunMat = new Materials::BlinnPhong();
    AModel portalGun(
      Resources::GetResource(RES_3D_MODEL, "PortalGun.fbx").c_str(),
      gunMat);
    portalGun.GetTransform().Scale(0.5, 0.5, 0.5);
    portalGun.GetTransform()
      .SetPositionAndRotation(0.f, 0.f, 0.f, 0.f, 90.f, 0.f);
    m_Scene->AddModel(std::move(portalGun));

    //==========================//
    // Grab GPU device metadata //
    //==========================//
    Profiler::Initialize();
    Profiler::Start();

    // ======================== //
    //  Enable post processing  //
    // ======================== //
    PostProcessing::Initialize();

    //==================//
    // Initialize ImGui //
    //==================//
    DebugUI::Initialize();
}

void DemoApp::Cleanup() {
    m_Scene->Destroy();
    delete m_Scene;
    PostProcessing::Shutdown();
    DebugUI::Shutdown();
    Profiler::Shutdown();
}

bool DemoApp::IsDone() {
    return glfwWindowShouldClose(Graphics::GetWindow());
}

void DemoApp::Update(const float deltaTime) {
    m_Scene->Update(deltaTime);
    DebugUI::Update(deltaTime);
    Profiler::Update();
}

void DemoApp::RenderScene() {
    m_Scene->Render();
    PostProcessing::Render();
}

void DemoApp::RenderUI() {
    if (m_ShowDebugOutput) {
        DebugUI::Draw();
    }
}

void DemoApp::LateUpdate(float deltaTime) {
    m_Scene->LateUpdate(deltaTime);
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