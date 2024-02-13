#include "Color.h"
#include "GraphicsContext.h"
#include "Input.h"
#include "Resources.h"
#include "GameApp.h"
#include "Shader.h"
#include "TextRenderer.h"
#include <fmt/format.h>

namespace DemoContent {
    float vertices[] = {
      0.5f,
      0.5f,
      0.0f,  // top right
      0.5f,
      -0.5f,
      0.0f,  // bottom right
      -0.5f,
      -0.5f,
      0.0f,  // bottom left
      -0.5f,
      0.5f,
      0.0f  // top left
    };
    uint32_t indices[] = {0, 1, 3, 1, 2, 3};
    float texCoords[]  = {
      0.0f,
      0.0f,  // lower-left corner
      1.0f,
      0.0f,  // lower-right corner
      0.5f,
      1.0f  // top-center corner
    };
    uint32_t VBO;
    uint32_t VAO;
    uint32_t EBO;
    AShader* shader;

    auto InitTestData() {
        shader = new AShader("Resources/Shaders/triangle.glsl");

        // Generate OpenGL Buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the Vertex Array Object first, then bind and set the vertex
        // buffer(s), and finally configure vertex attributes
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertices),
                     vertices,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(indices),
                     indices,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              3 * sizeof(float),
                              static_cast<void*>(nullptr));
        glEnableVertexAttribArray(0);

        // Note that this is allowed, the call to glVertexAttribPointer
        // registered VBO as the vertex attribute's bound vertex buffer object
        // so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound
        // element buffer object IS stored in the VAO; keep the EBO bound.
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't
        // accidentally modify this VAO, but this rarely happens. Modifying
        // other VAOs requires a call to glBindVertexArray anyways so we
        // generally don't unbind VAOs (nor VBOs) when it's not directly
        // necessary.
        glBindVertexArray(0);

        // Polygon mode (for wireframe)
        // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }

    auto RenderTestData() {
        shader->Use();

        const auto time   = static_cast<float>(glfwGetTime());
        const float green = (sin(time) / 2.0f) + 0.5f;
        shader->SetVec4("ourColor", 1.f, green, 0.f, 2.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    auto CleanupTestData() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        shader->Destroy();
        delete shader;
    }
}  // namespace DemoContent

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
    float m_FPS            = 0.f;
    bool m_ShowDebugOutput = true;

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

    Input::AddAction(GLFW_KEY_ENTER, GLFW_PRESS, GLFW_MOD_ALT, [&]() {
        Graphics::ToggleFullscreen();
    });

    // ========================= //
    //  Initialize demo content  //
    // ========================= //
    DemoContent::InitTestData();

    TextRenderer::LoadFont("ShareTechMono",
                           16,
                           "Resources/Fonts/ShareTechMono-Regular.ttf");
}

void DemoApp::Cleanup() {
    DemoContent::CleanupTestData();
}

bool DemoApp::IsDone() {
    return glfwWindowShouldClose(Graphics::GetWindow());
}

void DemoApp::Update(const float deltaTime) {
    m_FPS = 1.f / deltaTime;
}

void DemoApp::RenderScene() {
    DemoContent::RenderTestData();
}

void DemoApp::RenderUI() {
    TextRenderer::RenderText(
      "ShareTechMono",
      "[F12]       - Toggle render metrics",
      Graphics::Screen::TopLeft().Width,
      static_cast<float>(Graphics::Screen::TopLeft().Height) - (16 * 1),
      1.f,
      Colors::Black);
    TextRenderer::RenderText(
      "ShareTechMono",
      "[Alt+Enter] - Toggle fullscreen",
      Graphics::Screen::TopLeft().Width,
      static_cast<float>(Graphics::Screen::TopLeft().Height) - (16 * 2),
      1.f,
      Colors::Black);
    TextRenderer::RenderText(
      "ShareTechMono",
      "[Esc]       - Exit",
      Graphics::Screen::TopLeft().Width,
      static_cast<float>(Graphics::Screen::TopLeft().Height) - (16 * 3),
      1.f,
      Colors::Black);

    if (m_ShowDebugOutput) {
        TextRenderer::RenderText(
          "ShareTechMono",
          fmt::format("FPS  : {}", round(m_FPS)).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) - (16 * 5),
          1.f,
          DebugValueColor(29, 59));

        TextRenderer::RenderText(
          "ShareTechMono",
          fmt::format("Time : {:1.02f}ms", (1 / m_FPS) * 1000).c_str(),
          Graphics::Screen::TopLeft().Width,
          static_cast<float>(Graphics::Screen::TopLeft().Height) - (16 * 6),
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
    Application::InitializeApp(app, 1408, 792, "GLEngine");
    Application::RunApp(app);

    return 0;
}