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

#include <memory>
#include <Windows.h>

#include "GenericPlatform.h"
#include "WindowsClock.h"
#include "DX11/DX11RHI.h"

namespace XEN::Platform {

using std::unique_ptr;
using std::make_unique;
using RHI::DX11RHI;
    
class PLATFORM_API WindowsPlatform : public GenericPlatform {
public:
    void Initialize(ERHI rhi) override;
    void Run(std::function<void()> RunHook) override;
    void Shutdown() override;
    void* GetWindowHandle() override;
    inline HINSTANCE* GetWindowInstance() { return &mInstance; }
    [[nodiscard]] inline void* GetRHI() const {
        switch (mRHIType) {
            case ERHI::RHI_DX11:
                return mDX11_RHI.get();
            case ERHI::RHI_DX12:
                //return mDX12_RHI.get();
            case ERHI::RHI_VULKAN:
                return nullptr;
                //return mVulkan_RHI.get();
            default:
                return nullptr;
        }
    }

public:
    WindowsPlatform() = default;
    WindowsPlatform(WindowsPlatform const&);

public:
    LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    ERHI mRHIType = ERHI::RHI_DX11;
    LONG mWindowWidth = 1280;
    LONG mWindowHeight = 720;
    LPCWSTR mWindowClassName = L"WindowsPlatformClass";
    LPCWSTR mWindowName = L"XEN";
    HWND mWindowHandle = nullptr;
    HINSTANCE mInstance = nullptr;
    unique_ptr<WindowsClock> mClock = nullptr;
    unique_ptr<DX11RHI> mDX11_RHI = nullptr;
    // unique_ptr<DX12RHI> mDX12_RHI = nullptr;
    // unique_ptr<VulkanRHI> mVulkan_RHI = nullptr;
};

static unique_ptr<WindowsPlatform> ApplicationHandle = nullptr;
LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
 
}
