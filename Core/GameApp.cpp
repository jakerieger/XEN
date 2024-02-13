//
// Created by jr on 2/12/24.
//

#include "GameApp.h"
#include "GraphicsContext.h"
#include "Input.h"
#include "TextRenderer.h"

namespace Application {
    void InitializeApp(IGameApp& app,
                       const int& width,
                       const int& height,
                       const char* title) {
        Graphics::Initialize(FSize {width, height}, title);
        Input::Initialize(Graphics::GetWindow());
        TextRenderer::Initialize();
        app.Startup();
    }

    void TerminateApp(IGameApp& app) {
        app.Cleanup();
    }

    bool UpdateApp(IGameApp& app) {
        app.Update(Graphics::GetFrameTime());
        app.RenderScene();

        return !app.IsDone();
    }

    void RunApp(IGameApp& app) {
        while (UpdateApp(app)) {}
        TerminateApp(app);
        Graphics::Shutdown();
        TextRenderer::Shutdown();
    }
}  // namespace Application