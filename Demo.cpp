#include "Color.h"
#include "GraphicsContext.h"
#include "Input.h"
#include "Resources.h"
#include "GameApp.h"
#include "FullscreenQuad.h"
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

    TextRenderer::LoadFont(
      "JetBrainsMono",
      16,
      Resources::GetResource(RES_FONT, "JetBrainsMono-SemiBold.ttf").c_str());
}

void DemoApp::Cleanup() {
    m_FullscreenQuad->Destroy();
}

bool DemoApp::IsDone() {
    return glfwWindowShouldClose(Graphics::GetWindow());
}

void DemoApp::Update(const float deltaTime) {
    m_FPS = 1.f / deltaTime;
    m_FullscreenQuad->Update(deltaTime);
}

void DemoApp::RenderScene() {
    m_FullscreenQuad->Render();
}

void DemoApp::RenderUI() {
    constexpr int fontSize = 18;
    TextRenderer::RenderText(
      "JetBrainsMono",
      "[F10] - Toggle fullscreen",
      Graphics::Screen::TopLeft().Width,
      static_cast<float>(Graphics::Screen::TopLeft().Height) - (fontSize * 1),
      1.f,
      Colors::White);
    TextRenderer::RenderText(
      "JetBrainsMono",
      "[F11] - Toggle VSync",
      Graphics::Screen::TopLeft().Width,
      static_cast<float>(Graphics::Screen::TopLeft().Height) - (fontSize * 2),
      1.f,
      Colors::White);
    TextRenderer::RenderText(
      "JetBrainsMono",
      "[F12] - Toggle render metrics",
      Graphics::Screen::TopLeft().Width,
      static_cast<float>(Graphics::Screen::TopLeft().Height) - (fontSize * 3),
      1.f,
      Colors::White);
    TextRenderer::RenderText(
      "JetBrainsMono",
      "[Tab] - Toggle wireframe",
      Graphics::Screen::TopLeft().Width,
      static_cast<float>(Graphics::Screen::TopLeft().Height) - (fontSize * 4),
      1.f,
      Colors::White);
    TextRenderer::RenderText(
      "JetBrainsMono",
      "[Esc] - Exit",
      Graphics::Screen::TopLeft().Width,
      static_cast<float>(Graphics::Screen::TopLeft().Height) - (fontSize * 5),
      1.f,
      Colors::White);

    if (m_ShowDebugOutput) {
        TextRenderer::RenderText(
          "JetBrainsMono",
          fmt::format("FPS  : {}", round(m_FPS)).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) -
            (fontSize * 7),
          1.f,
          DebugValueColor(29, 59));

        TextRenderer::RenderText(
          "JetBrainsMono",
          fmt::format("Time : {:1.02f}ms", (1 / m_FPS) * 1000).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) -
            (fontSize * 8),
          1.f,
          DebugValueColor(29, 59));
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