#include "Color.h"
#include "GraphicsContext.h"
#include "Input.h"
#include "Resources.h"
#include "GameApp.h"
#include "FullscreenQuad.h"
#include "PostProcessing.h"
#include "Profiler.h"
#include "Shader.h"
#include "TextRenderer.h"
#include <fmt/format.h>

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

    FColor DebugValueColor(float err, float warn) const;

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
    m_FullscreenQuad = new AFullscreenQuad();
    m_FullscreenQuad->Initialize(BuiltinShaders::Quad);

    m_GpuVendor   = Profiler::GPU::GetDeviceVendor();
    m_GpuRenderer = Profiler::GPU::GetDeviceRenderer();

    TextRenderer::LoadFont(
      "JetBrainsMono",
      12,
      Resources::GetResource(RES_FONT, "JetBrainsMono-SemiBold.ttf").c_str());

    // ======================== //
    //  Enable post processing  //
    // ======================== //
    PostProcessing::Initialize();
}

void DemoApp::Cleanup() {
    m_FullscreenQuad->Destroy();
    PostProcessing::Shutdown();
}

bool DemoApp::IsDone() {
    return glfwWindowShouldClose(Graphics::GetWindow());
}

void DemoApp::Update(const float deltaTime) {
    m_FPS = 1.f / deltaTime;
    m_FullscreenQuad->Update(deltaTime);
    Profiler::GPU::GetMemoryUsage(m_TotalVram, m_UsedVram, m_FreeVram);
}

void DemoApp::RenderScene() {
    m_FullscreenQuad->Render();
    PostProcessing::Render();
}

void DemoApp::RenderUI() {
    //===========================================//
    // Draw debug output text in top-left corner //
    //===========================================//
    constexpr int fontSize = 14;  // add 2 to give the text a y margin
    if (m_ShowDebugOutput) {
        TextRenderer::RenderText(
          "JetBrainsMono",
          fmt::format("FPS          : {}", round(m_FPS)).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) -
            (fontSize * 1),
          1.f,
          DebugValueColor(29, 59));

        TextRenderer::RenderText(
          "JetBrainsMono",
          fmt::format("Time         : {:1.02f}ms", (1 / m_FPS) * 1000).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) -
            (fontSize * 2),
          1.f,
          DebugValueColor(29, 59));

        TextRenderer::RenderText(
          "JetBrainsMono",
          fmt::format("GPU Vendor   : {}", m_GpuVendor.c_str()).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) -
            (fontSize * 3),
          1.f,
          Colors::White);

        TextRenderer::RenderText(
          "JetBrainsMono",
          fmt::format("GPU Renderer : {}", m_GpuRenderer.c_str()).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) -
            (fontSize * 4),
          1.f,
          Colors::White);

        TextRenderer::RenderText(
          "JetBrainsMono",
          fmt::format("Total Memory : {:.2f} MB", m_TotalVram / 1000.f).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) -
            (fontSize * 5),
          1.f,
          Colors::Cyan);

        TextRenderer::RenderText(
          "JetBrainsMono",
          fmt::format("Used Memory  : {:.2f} MB", m_UsedVram / 1000.f).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) -
            (fontSize * 6),
          1.f,
          Colors::Cyan);

        TextRenderer::RenderText(
          "JetBrainsMono",
          fmt::format("Free Memory  : {:.2f} MB", m_FreeVram / 1000.f).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) -
            (fontSize * 7),
          1.f,
          Colors::Cyan);
    }
}

void DemoApp::LateUpdate(float deltaTime) {}

FColor DemoApp::DebugValueColor(const float err, const float warn) const {
    if (m_FPS < err) {
        return Colors::Red;
    }
    if (m_FPS < warn) {
        return Colors::Yellow;
    }
    return Colors::Green;
}

int main(int argc, char* argv[]) {
    Resources::SetCwd(argv[0]);

    DemoApp app;
    Application::InitializeApp(app, 1408, 792, "GLEngine | DemoApp");
    Application::RunApp(app);

    return 0;
}