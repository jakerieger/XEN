#include "GraphicsContext.h"

#include <GameApp.h>

class GLEngine final : public IGameApp {
public:
    void Startup() override;
    void Cleanup() override;
    bool IsDone() override;
    void Update(float deltaTime) override;
    void RenderScene() override;

private:
    //====================================//
    // Put application-specific data here //
    //====================================//
};

void GLEngine::Startup() {}

void GLEngine::Cleanup() {}

bool GLEngine::IsDone() {
    return glfwWindowShouldClose(Graphics::GetWindow());
}

void GLEngine::Update(const float deltaTime) {}

void GLEngine::RenderScene() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(Graphics::GetWindow());
    glfwPollEvents();
}

int main() {
    GLEngine app;

    Application::InitializeApp(app, 1600, 900, "GLEngine");
    Application::RunApp(app);

    return 0;
}