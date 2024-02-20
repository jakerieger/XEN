//
// Created by jr on 2/12/24.
//

#include "GameApp.h"
#include "GraphicsContext.h"
#include "GraphicsError.h"
#include "Color.h"
#include "DebugUI.h"
#include "Profiler.h"

namespace Application {
    FColor g_ClearColor = FColor(0xFF11131C);

    void InitializeApp(IGameApp& app, const FSize& size, const char* title) {
        Graphics::Initialize(size, title);
        Input::Initialize(Graphics::GetWindow());
        Profiler::Initialize();
        DebugUI::Initialize();

#ifdef _DEBUG_GRAPHICS
        Graphics::Error::EnableDebugOutput();
#endif

        Profiler::Start();
        app.Startup();
    }

    bool UpdateApp(IGameApp& app) {
        // Update first
        const auto activeScene = app.GetActiveScene();
        const float frameTime  = Graphics::GetFrameTime();
        if (activeScene) {
            activeScene->Update(frameTime);
        }

        Profiler::Update();
        DebugUI::Update(frameTime, activeScene);

        // Clear buffers
        glClearColor(g_ClearColor.Red,
                     g_ClearColor.Green,
                     g_ClearColor.Blue,
                     g_ClearColor.Alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Execute user-defined render logic
        glEnable(GL_DEPTH_TEST);
        if (activeScene) {
            activeScene->Render();
        }
        DebugUI::Draw();

        // Swap buffers and poll events
        glfwSwapBuffers(Graphics::GetWindow());
        glfwPollEvents();

        // Execute user late update logic
        if (activeScene) {
            activeScene->LateUpdate();
        }

        // TODO: implement a fixed update loop
        // app.FixedUpdate();

        return !glfwWindowShouldClose(Graphics::GetWindow());
    }

    void RunApp(IGameApp& app) {
        while (UpdateApp(app)) {}
        app.Cleanup();
        Graphics::Shutdown();
        DebugUI::Shutdown();
        Profiler::Shutdown();
    }
}  // namespace Application

void IGameApp::AddScene(eastl::unique_ptr<AScene> scene) {
    m_Scenes.push_back(eastl::move(scene));
}

void IGameApp::LoadScene(const eastl::string& name) {
    const auto scene = GetScene(name);
    if (scene) {
        scene->SetActive(true);
        scene->Start();
    }
}