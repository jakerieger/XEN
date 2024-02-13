#include "Color.h"
#include "GraphicsContext.h"
#include "Resources.inl"

#include <GameApp.h>

class DemoApp final : public IGameApp {
public:
    DemoApp() = default;
    void Startup() override;
    void Cleanup() override;
    bool IsDone() override;
    void Update(float deltaTime) override;
    void RenderScene() override;

private:
    //====================================//
    // Put application-specific data here //
    //====================================//
    FColor clearColor = FColor(0xFF11131C);
};

void DemoApp::Startup() {}

void DemoApp::Cleanup() {}

bool DemoApp::IsDone() {
    return glfwWindowShouldClose(Graphics::GetWindow());
}

void DemoApp::Update(const float deltaTime) {}

void DemoApp::RenderScene() {
    glClearColor(clearColor.Red,
                 clearColor.Green,
                 clearColor.Blue,
                 clearColor.Alpha);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(Graphics::GetWindow());
    glfwPollEvents();
}

int main(int argc, char* argv[]) {
    Resources::SetCwd(argv[0]);

    DemoApp app;
    Application::InitializeApp(app, 1600, 900, "GLEngine");
    Application::RunApp(app);

    return 0;
}