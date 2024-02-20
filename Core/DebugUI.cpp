//
// Created by conta on 2/15/2024.
//

#include "DebugUI.h"

#include "GraphicsContext.h"
#include "Profiler.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace DebugUI {
    float g_FPS;
    float g_FrameTime;
    AScene* g_ActiveScene;
    bool g_Visible = true;

    void Initialize() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplGlfw_InitForOpenGL(Graphics::GetWindow(), true);
        ImGui_ImplOpenGL3_Init();
    }

    void Update(const float deltaTime, AScene* activeScene) {
        g_FPS         = 1.f / deltaTime;
        g_FrameTime   = (1.f / g_FPS) * 1000.f;  // in milliseconds
        g_ActiveScene = activeScene;
    }

    void Draw() {
        if (g_Visible) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Text("FPS          : %0.2f", g_FPS);
            ImGui::Text("Time         : %0.2fms", g_FrameTime);
            ImGui::Text("GPU Vendor   : %s", Profiler::GpuVendor.c_str());
            ImGui::Text("GPU Renderer : %s", Profiler::GpuRenderer.c_str());
            ImGui::Text("Total Mem    : %0.2f MB",
                        Profiler::TotalMemory / 1000);
            ImGui::Text("Used Mem     : %0.2f MB", Profiler::UsedMemory / 1000);
            ImGui::Text("Free Mem     : %0.2f MB", Profiler::FreeMemory / 1000);

            ImGui::Separator();

            if (g_ActiveScene) {
                ImGui::Text("Active Scene");
                ImGui::Text("Name         : %s",
                            g_ActiveScene->GetName().c_str());
                ImGui::Text(
                  "Game Objects : %d",
                  static_cast<int>(
                    g_ActiveScene->GetContext().m_GameObjects.size()));
                ImGui::Text("Triangles    : %d",
                            Profiler::GetTotalTriangles(g_ActiveScene));
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }

    void Shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ToggleVisible() {
        g_Visible = !g_Visible;
    }
}  // namespace DebugUI