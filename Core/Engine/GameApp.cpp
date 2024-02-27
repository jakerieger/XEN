//
// Created by jr on 2/12/24.
//

#include "GameApp.h"
#include "GraphicsContext.h"
#include "GraphicsError.h"
#include "Color.h"
#include "DebugUI.h"
#include "Profiler.h"
#include "Quad.h"
#include "SkyboxRenderPass.h"

namespace Application {
    FColor g_ClearColor = FColor(0xFF11131C);

    ASkyboxRenderPass g_SkyboxRenderPass;
    AQuad g_DebugQuad;
    // =========================================================

    void InitializeApp(IGameApp& app,
                       const FSize& size,
                       const char* title,
                       const bool launchFullscreen) {
        Graphics::Initialize(size, title);
        if (launchFullscreen) {
            Graphics::ToggleFullscreen();
        }
        Input::Initialize(Graphics::GetWindow());
        Profiler::Initialize();
        DebugUI::Initialize();

        g_SkyboxRenderPass.Init();
        g_DebugQuad.Init();

#ifdef _DEBUG_GRAPHICS
        Graphics::Error::EnableDebugOutput();
#endif

        Profiler::Start();

        app.Startup();
    }

    bool UpdateApp(IGameApp& app) {
        Graphics::ResetDrawCalls();
        Graphics::UpdateFrameTime();

        // FixedUpdate time step is a constant 1000 FPS
        // Used for polling IO events and physics simulations
        constexpr float FIXED_TIME_STEP = 1.0 / 1000.0;
        static float accumulatedTime    = 0.0;
        const float frameTime           = Graphics::GetDeltaTime();
        accumulatedTime += frameTime;

        const auto activeScene = app.GetActiveScene();

        // Fixed update loop
        while (accumulatedTime >= FIXED_TIME_STEP) {
            if (activeScene) {
                activeScene->FixedUpdated();
            }

            accumulatedTime -= FIXED_TIME_STEP;
        }

        // Update scene
        if (activeScene) {
            activeScene->Update(frameTime);
        }

        // Update engine analytics
        Profiler::Update();
        DebugUI::Update(frameTime, activeScene);

        // Clear buffers
        glClearColor(g_ClearColor.Red,
                     g_ClearColor.Green,
                     g_ClearColor.Blue,
                     g_ClearColor.Alpha);

        // Render scene + UI
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);

        if (activeScene) {
            const auto camera =
              AScene::GetActiveCamera(activeScene->GetContext());

            if (camera) {
                activeScene->Render();
                g_SkyboxRenderPass.Render(camera);
                // g_DebugQuad.Draw(activeScene->GetContext().DepthMap, 1.f, 7.5f);
            }
        }

        DebugUI::Draw();

        // Swap buffers and poll events
        glfwSwapBuffers(Graphics::GetWindow());

        // Execute user late update logic
        if (activeScene) {
            activeScene->LateUpdate();
        }

        glfwPollEvents();
        return !glfwWindowShouldClose(Graphics::GetWindow());
    }

    void RunApp(IGameApp& app) {
        while (UpdateApp(app)) {}

        app.Cleanup();
        Graphics::Shutdown();
        DebugUI::Shutdown();
        Profiler::Shutdown();

        g_SkyboxRenderPass.Cleanup();
        g_DebugQuad.Cleanup();
    }
}  // namespace Application

void IGameApp::AddScene(eastl::unique_ptr<AScene>& scene) {
    m_Scenes.push_back(move(scene));
}

void IGameApp::LoadScene(const eastl::string& name) {
    const auto scene        = GetScene(name);
    const auto currentScene = GetActiveScene();
    if (currentScene) {
        UnloadScene(currentScene);
    }

    if (scene) {
        scene->SetActive(true);
        scene->Start();

        for (auto& go : scene->GetContext().GameObjects) {
            Input::RegisterListener(go.get());
        }
    }
}

void IGameApp::UnloadScene(AScene* scene) {
    Input::UnregisterSceneListeners(this);

    // TODO: Intead of initializing gameobjects in their constructor,
    // move the logic to the Awake method so that gameobjects
    // can be reinitialized on scene load and not permanently
    // destroyed when unloading a scene
    // scene->Destroyed();
    scene->SetActive(false);
}