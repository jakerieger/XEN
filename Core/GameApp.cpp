//
// Created by jr on 2/12/24.
//

#include "GameApp.h"
#include "GraphicsContext.h"

namespace Application {
    void InitializeApp(IGameApp& app,
                       const int& width,
                       const int& height,
                       const char* title) {
        Graphics::Initialize(FSize {width, height}, title);
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
    }
}  // namespace Application