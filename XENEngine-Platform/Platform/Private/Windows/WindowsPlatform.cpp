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

#include "Windows/WindowsPlatform.h"
#include <cassert>

namespace XEN::Platform {

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        default:
            return ApplicationHandle->MessageHandler(hwnd, msg, wparam, lparam);
    }
}

void WindowsPlatform::Initialize(ERHI rhi) {
    mRHIType = rhi;
    
    mClock = make_unique<WindowsClock>(WindowsClock{});
    assert(mClock.get());
    
    WNDCLASSEXW window_class;
    int posX, posY;

    ApplicationHandle = make_unique<WindowsPlatform>(*this);
    mInstance = GetModuleHandle(NULL);

    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = WndProc;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = mInstance;
    window_class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    window_class.hIconSm = window_class.hIcon;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    window_class.lpszMenuName = NULL;
    window_class.lpszClassName = mWindowClassName;
    window_class.cbSize = sizeof(WNDCLASSEXW);

    static ATOM atom = RegisterClassExW(&window_class);
    assert(atom > 0);

    int scr_width, scr_height;
    scr_width = GetSystemMetrics(SM_CXSCREEN);
    scr_height = GetSystemMetrics(SM_CYSCREEN);

    posX = (scr_width - mWindowWidth) / 2;
    posY = (scr_height - mWindowHeight) / 2;

    mWindowHandle = CreateWindowExW(
        NULL,
        mWindowClassName,
        mWindowName,
        WS_OVERLAPPEDWINDOW,
        posX,
        posY,
        mWindowWidth,
        mWindowHeight,
        nullptr,
        nullptr,
        mInstance,
        nullptr
    );

    assert(mWindowHandle != nullptr);

    ShowWindow(mWindowHandle, SW_SHOW);
    UpdateWindow(mWindowHandle);

    // Setup RHI
    switch (rhi) {
        case ERHI::RHI_DX11: {
            mDX11_RHI = make_unique<DX11RHI>(DX11RHI{});
            assert(mDX11_RHI != nullptr);

            mDX11_RHI->Initialize(&mWindowHandle, &mInstance);
            
            break;
        }
        case ERHI::RHI_DX12:
        case ERHI::RHI_VULKAN: {
            break;
        }
    }
    
    mClock->StartClock();
    mClock->mLastTime = mClock->GetTime();
}

void WindowsPlatform::Run(std::function<void()> RunHook) {
    assert(mClock.get());
    
    MSG msg = {0};

    // Awake

    // Start

    while (msg.message != WM_QUIT) {
        double deltaTime = mClock->GetTime() - mClock->mLastTime;
        
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Game Loop
            switch (mRHIType) {
                case ERHI::RHI_DX11: {
                    RunHook();
                    mDX11_RHI->Run();
                    break;
                }
                case ERHI::RHI_DX12:
                case ERHI::RHI_VULKAN:
                    break;
            }
        }

        mClock->mLastTime = mClock->GetTime();
    }

    // Destroyed
    switch (mRHIType) {
        case ERHI::RHI_DX11: {
            mDX11_RHI->Shutdown();
            break;
        }
        case ERHI::RHI_DX12:
        case ERHI::RHI_VULKAN:
            break;
    }
}

void WindowsPlatform::Shutdown() {
    DestroyWindow(mWindowHandle);
    mWindowHandle = nullptr;
}

void* WindowsPlatform::GetWindowHandle() {
    return mWindowHandle;
}

WindowsPlatform::WindowsPlatform(WindowsPlatform const& other) {}

LRESULT WindowsPlatform::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    PAINTSTRUCT paint_struct;
    HDC hdc;

    switch (msg) {
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &paint_struct);
            EndPaint(hwnd, &paint_struct);
            return 0;
        }
        case WM_KEYDOWN:
        case WM_KEYUP: {
            return 0;
        }
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}
    
}
