//
// Created by jr on 2/12/24.
//

#include "GameApp.h"
#include "GraphicsContext.h"
#include "GraphicsError.h"
#include "Color.h"
#include "DebugUI.h"
#include "Framebuffer.h"
#include "Profiler.h"
#include "Skybox.h"

#include <Shader.h>

namespace Application {
    FColor g_ClearColor = FColor(0xFF11131C);

    float g_QuadVertices[] = {-1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                              0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                              -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                              1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};
    u32 g_QuadVAO;
    u32 g_QuadVBO;

    u32 g_DepthMapFBO;
    u32 g_DepthMap;

    constexpr u32 SHADOW_WIDTH  = 1024;
    constexpr u32 SHADOW_HEIGHT = 1024;

    ASkybox* g_Skybox;
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

        g_Skybox = new ASkybox();
        g_Skybox->Init();

        //================= SHADOW MAPS =================//
        // glGenFramebuffers(1, &g_DepthMapFBO);
        // glGenTextures(1, &g_DepthMap);
        // glBindTexture(GL_TEXTURE_2D, g_DepthMap);
        // glTexImage2D(GL_TEXTURE_2D,
        //              0,
        //              GL_DEPTH_COMPONENT,
        //              SHADOW_WIDTH,
        //              SHADOW_HEIGHT,
        //              0,
        //              GL_DEPTH_COMPONENT,
        //              GL_FLOAT,
        //              nullptr);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //
        // glBindFramebuffer(GL_FRAMEBUFFER, g_DepthMapFBO);
        // glFramebufferTexture2D(GL_FRAMEBUFFER,
        //                        GL_DEPTH_ATTACHMENT,
        //                        GL_TEXTURE_2D,
        //                        g_DepthMap,
        //                        0);
        // glDrawBuffer(GL_NONE);
        // glReadBuffer(GL_NONE);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //===============================================//

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
                glfwPollEvents();
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

        //=======================================//
        // Render to post-processing framebuffer //
        //=======================================//

        // Clear buffers
        glClearColor(g_ClearColor.Red,
                     g_ClearColor.Green,
                     g_ClearColor.Blue,
                     g_ClearColor.Alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene + UI
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);
        // glFrontFace(GL_CW);

        if (activeScene) {
            activeScene->Render();
            g_Skybox->Draw(AScene::GetActiveCamera(activeScene->GetContext()));
        }

        //==============================//
        // Render to screen framebuffer //
        //==============================//

        DebugUI::Draw();

        // Swap buffers and poll events
        glfwSwapBuffers(Graphics::GetWindow());

        // Execute user late update logic
        if (activeScene) {
            activeScene->LateUpdate();
        }

        return !glfwWindowShouldClose(Graphics::GetWindow());
    }

    void RunApp(IGameApp& app) {
        while (UpdateApp(app)) {}

        app.Cleanup();
        Graphics::Shutdown();
        DebugUI::Shutdown();
        Profiler::Shutdown();

        g_Skybox->Destroy();
        delete g_Skybox;
    }
}  // namespace Application

void IGameApp::AddScene(eastl::unique_ptr<AScene>& scene) {
    m_Scenes.push_back(move(scene));
}

void IGameApp::LoadScene(const eastl::string& name) {
    const auto scene = GetScene(name);
    if (scene) {
        scene->SetActive(true);
        scene->Start();

        for (auto& go : scene->GetContext().m_GameObjects) {
            Input::RegisterListener(go.get());
        }
    }
}