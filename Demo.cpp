#include "Camera.h"
#include "Color.h"
#include "DebugUI.h"
#include "DirectionalLight.h"
#include "GraphicsContext.h"
#include "Input.h"
#include "Resources.h"
#include "GameApp.h"
#include "Model.h"
#include "PostProcessing.h"
#include "Profiler.h"
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
    bool m_ShowDebugOutput        = true;
    AModel* m_Model               = nullptr;
    ACamera* m_Camera             = nullptr;
    ADirectionalLight* m_DirLight = nullptr;
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
    m_DirLight = new ADirectionalLight();
    m_DirLight->GetTransform().Translate(0.f, 0.f, -2.f);
    m_Camera = new ACamera(glm::vec3(0.f, 0.f, -5.f));
    AShader modelShader(BuiltinShaders::BlinnPhong);

    m_Model =
      new AModel(Resources::GetResource(RES_3D_MODEL, "monk.fbx").c_str(),
                 modelShader);

    m_Model->GetTransform().Scale(0.01, 0.01, 0.01);
    m_Model->GetShader().Use();
    m_Model->GetShader().SetMat4(
      "u_Projection",
      ACamera::GetProjectionMatrix(45.f, Graphics::GetWindowAspect()));
    m_Model->GetShader().SetMat4("u_View", m_Camera->GetViewMatrix());
    m_Model->GetShader().SetMat4("u_Model",
                                 m_Model->GetTransform().GetModelMatrix());
    m_Model->GetShader().SetVec3("u_ObjectColor", {0.5f, 0.5f, 0.5f});
    m_Model->GetShader().SetVec3("u_LightColor", m_DirLight->GetColor());
    m_Model->GetShader().SetVec3("u_LightPosition",
                                 m_DirLight->GetTransform().GetPosition());
    m_Model->GetShader().SetVec3("u_ViewPosition", m_Camera->GetPosition());

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
    delete m_Camera;
    m_Model->Destroy();
    delete m_Model;
    PostProcessing::Shutdown();
    DebugUI::Shutdown();
    Profiler::Shutdown();
}

bool DemoApp::IsDone() {
    return glfwWindowShouldClose(Graphics::GetWindow());
}

void DemoApp::Update(const float deltaTime) {
    // Rotate our monk model
    const auto rotationY = 50 * (glfwGetTime() * glm::radians(45.f));
    m_Model->GetTransform().SetPositionAndRotation(
      glm::vec3(0.f),
      glm::vec3(0.f, rotationY, 0.f));
    m_Model->Update(deltaTime);
    m_DirLight->GetTransform().Update();

    DebugUI::Update(deltaTime);
    Profiler::Update();
}

void DemoApp::RenderScene() {
    m_Model->Draw();
    PostProcessing::Render();
}

void DemoApp::RenderUI() {
    if (m_ShowDebugOutput) {
        DebugUI::Draw();
    }
}

void DemoApp::LateUpdate(float deltaTime) {}

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