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

#include <Shader.h>

namespace Application {
    FColor g_ClearColor = FColor(0xFF11131C);

    // All of the below variables are used for post processing
    AFramebuffer g_PPBuffer;
    AShader* g_PPShader;
    float g_QuadVertices[] = {-1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                              0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                              -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                              1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};
    u32 g_QuadVAO;
    u32 g_QuadVBO;
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

        g_PPBuffer.Initialize();
        g_PPShader = new AShader(BuiltinShaders::Quad);
        glGenVertexArrays(1, &g_QuadVAO);
        glGenBuffers(1, &g_QuadVBO);
        glBindVertexArray(g_QuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, g_QuadVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(g_QuadVertices),
                     &g_QuadVertices,
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              4 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              4 * sizeof(float),
                              (void*)(2 * sizeof(float)));

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
        g_PPBuffer.Bind();

        // Clear buffers
        glClearColor(g_ClearColor.Red,
                     g_ClearColor.Green,
                     g_ClearColor.Blue,
                     g_ClearColor.Alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene + UI
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);

        if (activeScene) {
            activeScene->Render();
        }

        //==============================//
        // Render to screen framebuffer //
        //==============================//
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(
          1.0f,
          1.0f,
          1.0f,
          1.0f);  // set clear color to white (not really necessary actually,
                  // since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        // Enable PP Shader
        g_PPShader->Use();
        glBindVertexArray(g_QuadVAO);
        glBindTexture(GL_TEXTURE_2D, g_PPBuffer.GetTextureBuffer());
        g_PPShader->SetInt("u_Texture", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

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
        delete g_PPShader;
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