/**
* Copyright 2023 Jake Rieger
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "DX11/DX11Includes.h"
#include "RHI.h"
#include "Color.h"

#include <Windows.h>
#include <memory>

#include "DX11RenderContext.h"

namespace XEN::RHI {

using std::make_unique;
using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using namespace XEN::Core;
    
class RHI_API DX11RHI {
public:
    DX11RHI() = default;

public:
    void Initialize(HWND* WindowHandle, HINSTANCE* WindowInstance);
    // Runs at game time
    void Update();
    // Runs at a fixed timestep - used for physics
    void FixedUpdate();
    // Runs at game time post-render
    void LateUpdate();
    void Render();
    void Shutdown();

    void Run();
    void CreateRenderObject();
    [[nodiscard]] inline HWND* GetWindowHandle() const { return mWindowHandle; }
    [[nodiscard]] FDX11RenderContext& GetRenderContext() { return mRenderContext; }

protected:
    void Present();
    void Clear(const FColor& clearColor, float clearDepth, uint8 clearStencil);
    DXGI_RATIONAL QueryRefreshRate(uint32 screenWidth, uint32 screenHeight, bool vSync);

private:
    HWND* mWindowHandle = nullptr;
    HINSTANCE* mWindowInstance = nullptr;
    FDX11RenderContext mRenderContext;
};
    
}
