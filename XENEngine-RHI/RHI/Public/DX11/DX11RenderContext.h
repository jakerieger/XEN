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

#include "RHI.h"
#include "DX11Includes.h"
#include "DX11RenderObject.h"

namespace XEN::RHI {

using std::unique_ptr;
using std::make_unique;

class DX11RHI;
    
struct RHI_API FDX11RenderContext {
    ID3D11Device* mDevice = nullptr;
    ID3D11DeviceContext* mDeviceContext = nullptr;
    IDXGISwapChain* mSwapChain = nullptr;
    ID3D11RenderTargetView* mRenderTargetView = nullptr;
    ID3D11DepthStencilView* mDepthStencilView = nullptr;
    ID3D11Texture2D* mDepthStencilBuffer = nullptr;
    ID3D11DepthStencilState* mDepthStencilState = nullptr;
    ID3D11RasterizerState* mRasterizerState = nullptr;
    D3D11_VIEWPORT mViewport = {0};
    XMMATRIX mProjectionMatrix = {};
    XMMATRIX mViewMatrix = {};
    std::vector<FDX11RenderObject> mRenderObjects = {};
    DX11RHI* mOwningRHI = nullptr;

    void Initialize();
    void Release();
};
 
}
