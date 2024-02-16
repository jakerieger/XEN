# GLEngine

Just another game engine project based on OpenGL. Provides a `Core` static library that can
be linked against to build games and other 3D tools.

To see GLEngine in action, build and run the `DemoApp` target. This isn't a comprehensive
implementation of all of GLEngine's capabilities, but covers the majority of use-cases.

## Example Game Boilerplate

```c++
#include "GameApp.h"

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
};

void DemoApp::Startup() {
    //====================================//
    // Initialize game-specific data here //
    //====================================//
}

void DemoApp::Cleanup() {
    //====================================//
    // Cleanup and destroy game data here //
    //====================================//
}

bool DemoApp::IsDone() {
    return glfwWindowShouldClose(Graphics::GetWindow());
}

void DemoApp::Update(const float deltaTime) {
    //=====================================//
    // Update loop called before rendering //
    //=====================================//
}

void DemoApp::RenderScene() {
    //=====================//
    // Main rendering loop //
    //=====================//
}

void DemoApp::RenderUI() {
    //===============================================//
    // UI rendering loop, called after RenderScene() //
    //===============================================//
}

void DemoApp::LateUpdate(float deltaTime) {
    //=====================================================//
    // Update loop called after all rendering is completed //
    //=====================================================//
}

int main(int argc, char* argv[]) {
    // Required to load assets from the "Resources" directory
    // located in the game exe root directory
    Resources::SetCwd(argv[0]);

    DemoApp app;
    Application::InitializeApp(app,
                               { 1280, 720 },
                               "GLEngine | Example");
    Application::RunApp(app);

    return 0;
}
```

## License

GLEngine is licensed under the [ISC License](LICENSE). Third party files includes are all licensed
under their respective licenses.