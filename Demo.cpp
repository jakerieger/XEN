#include "Camera.h"
#include "Color.h"
#include "GraphicsContext.h"
#include "Input.h"
#include "Resources.h"
#include "GameApp.h"
#include "FullscreenQuad.h"
#include "Model.h"
#include "PostProcessing.h"
#include "Profiler.h"
#include "Shader.h"
#include "TextRenderer.h"
#include <fmt/format.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define SCREEN_720P                                                            \
    FSize {                                                                    \
        1280, 720                                                              \
    }
#define SCREEN_792P                                                            \
    FSize {                                                                    \
        1408, 729                                                              \
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
    float m_FPS                       = 0.f;
    bool m_ShowDebugOutput            = true;
    AFullscreenQuad* m_FullscreenQuad = nullptr;
    AModel* m_Cube                    = nullptr;
    ACamera* m_Camera                 = nullptr;

    // GPU statistics
    string m_GpuVendor;
    string m_GpuRenderer;
    float m_TotalVram;
    float m_UsedVram;
    float m_FreeVram;
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
    // m_FullscreenQuad = new AFullscreenQuad();
    // m_FullscreenQuad->Initialize(BuiltinShaders::Quad);
    AShader cubeShader(BuiltinShaders::Unlit);
    m_Cube =
      new AModel(Resources::GetResource(RES_3D_MODEL, "cube.obj").c_str(),
                 cubeShader);

    //==========================//
    // Grab GPU device metadata //
    //==========================//
    m_GpuVendor   = Profiler::GPU::GetDeviceVendor();
    m_GpuRenderer = Profiler::GPU::GetDeviceRenderer();

    // ======================== //
    //  Enable post processing  //
    // ======================== //
    PostProcessing::Initialize();

    //==================//
    // Initialize ImGui //
    //==================//
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(Graphics::GetWindow(), true);
    ImGui_ImplOpenGL3_Init();
}

void DemoApp::Cleanup() {
    // m_FullscreenQuad->Destroy();
    m_Cube->Destroy();
    PostProcessing::Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool DemoApp::IsDone() {
    return glfwWindowShouldClose(Graphics::GetWindow());
}

void DemoApp::Update(const float deltaTime) {
    m_FPS = 1.f / deltaTime;
    // m_FullscreenQuad->Update(deltaTime);
    Profiler::GPU::GetMemoryUsage(m_TotalVram, m_UsedVram, m_FreeVram);
}

void DemoApp::RenderScene() {
    // m_FullscreenQuad->Render();
    m_Cube->Draw();
    PostProcessing::Render();
}

void DemoApp::RenderUI() {
    //===========================================//
    // Draw debug output text in top-left corner //
    //===========================================//
    if (m_ShowDebugOutput) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Text("FPS          : %0.2f", m_FPS);
        ImGui::Text("Time         : %0.2fms", (1.f / m_FPS) * 1000.f);
        ImGui::Text("GPU Vendor   : %s", m_GpuVendor.c_str());
        ImGui::Text("GPU Renderer : %s", m_GpuRenderer.c_str());
        ImGui::Text("Total Mem    : %0.2f MB", m_TotalVram / 1000);
        ImGui::Text("Used Mem     : %0.2f MB", m_UsedVram / 1000);
        ImGui::Text("Free Mem     : %0.2f MB", m_FreeVram / 1000);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void DemoApp::LateUpdate(float deltaTime) {}

int main(int argc, char* argv[]) {
    Resources::SetCwd(argv[0]);

    DemoApp app;
    Application::InitializeApp(app, SCREEN_792P, "GLEngine | DemoApp");
    Application::RunApp(app);

    return 0;
}