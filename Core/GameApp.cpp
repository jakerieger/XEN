//
// Created by jr on 2/12/24.
//

#include "GameApp.h"
#include "GraphicsContext.h"
#include "GraphicsError.h"
#include "Input.h"
#include "PostProcessing.h"

namespace Application {
    FColor g_ClearColor = FColor(0xFF11131C);

    void InitializeApp(IGameApp& app, const FSize& size, const char* title) {
        Graphics::Initialize(size, title);
        Input::Initialize(Graphics::GetWindow());

#ifdef _DEBUG_GRAPHICS
        Graphics::Error::EnableDebugOutput();
#endif

        app.Startup();
    }

    void TerminateApp(IGameApp& app) {
        app.Cleanup();
    }

    bool UpdateApp(IGameApp& app) {
        // Update first
        const float frameTime = Graphics::GetFrameTime();
        app.Update(frameTime);

        // Clear buffers
        glClearColor(g_ClearColor.Red,
                     g_ClearColor.Green,
                     g_ClearColor.Blue,
                     g_ClearColor.Alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Execute user-defined render logic
        glEnable(GL_DEPTH_TEST);
        app.RenderScene();
        app.RenderUI();

        // Swap buffers and poll events
        glfwSwapBuffers(Graphics::GetWindow());
        glfwPollEvents();

        // Execute user late update logic
        app.LateUpdate(frameTime);

        return !app.IsDone();
    }

    void RunApp(IGameApp& app) {
        while (UpdateApp(app)) {}
        TerminateApp(app);
        Graphics::Shutdown();
    }
}  // namespace Application