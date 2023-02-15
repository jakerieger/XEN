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

#include "DX11/DX11RHI.h"

#include <thread>

#include "Windows/WindowsHelpers.inl"

using namespace XEN::RHI;

void DX11RHI::Initialize(HWND* WindowHandle, HINSTANCE* WindowInstance) {
    assert(WindowHandle != NULL);
    assert(WindowInstance != NULL);

    mWindowHandle = WindowHandle;
    mWindowInstance = WindowInstance;
    mRenderContext.mOwningRHI = this;

    RECT clientRect;
    GetClientRect(*mWindowHandle, &clientRect);

    unsigned clientWidth = clientRect.right - clientRect.left;
    unsigned clientHeight = clientRect.bottom - clientRect.top;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = clientWidth;
    swapChainDesc.BufferDesc.Height = clientHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate = QueryRefreshRate(clientWidth, clientHeight, false);
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = *mWindowHandle;
    swapChainDesc.SampleDesc.Count = 2;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Windowed = TRUE;

    uint32 createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] = 
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, 
        nullptr, createDeviceFlags, featureLevels, _countof(featureLevels), 
        D3D11_SDK_VERSION, &swapChainDesc, &mRenderContext.mSwapChain, &mRenderContext.mDevice, &featureLevel,
        &mRenderContext.mDeviceContext );

    if (hr == E_INVALIDARG) {
        hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, 
            nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1, 
            D3D11_SDK_VERSION, &swapChainDesc, &mRenderContext.mSwapChain, &mRenderContext.mDevice, &featureLevel,
            &mRenderContext.mDeviceContext);
    }
    
    assert(SUCCEEDED(hr));

    ID3D11Texture2D* backBuffer;
    hr = mRenderContext.mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    assert(SUCCEEDED(hr));

    hr = mRenderContext.mDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderContext.mRenderTargetView);
    assert(SUCCEEDED(hr));

    Platform::Windows::SafeRelease(backBuffer);

    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.Width = clientWidth;
    depthStencilBufferDesc.Height = clientHeight;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.SampleDesc.Count = 2;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    hr = mRenderContext.mDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &mRenderContext.mDepthStencilBuffer);
    assert(SUCCEEDED(hr));

    CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(CD3D11_DEPTH_STENCIL_VIEW_DESC));
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    hr = mRenderContext.mDevice->CreateDepthStencilView(mRenderContext.mDepthStencilBuffer, &dsvDesc, &mRenderContext.mDepthStencilView);
    assert(SUCCEEDED(hr));

    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory( &depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC) );

    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDesc.StencilEnable = FALSE;
    
    hr = mRenderContext.mDevice->CreateDepthStencilState(&depthStencilStateDesc, &mRenderContext.mDepthStencilState);
    assert(SUCCEEDED(hr));

    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.MultisampleEnable = TRUE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;

    hr = mRenderContext.mDevice->CreateRasterizerState(&rasterizerDesc, &mRenderContext.mRasterizerState);
    assert(SUCCEEDED(hr));

    mRenderContext.mViewport.Width = static_cast<float>(clientWidth);
    mRenderContext.mViewport.Height = static_cast<float>(clientHeight);
    mRenderContext.mViewport.TopLeftX = 0.0f;
    mRenderContext.mViewport.TopLeftY = 0.0f;
    mRenderContext.mViewport.MinDepth = 0.0f;
    mRenderContext.mViewport.MaxDepth = 1.0f;

    mRenderContext.Initialize();
}

void DX11RHI::Update() {
    for (auto& renderObject : mRenderContext.mRenderObjects) {
        if (!renderObject.mIsInitialized) {
            renderObject.Initialize();
        }

        renderObject.Update(1.f);
    }
}

void DX11RHI::FixedUpdate() {
    for (auto& renderObject : mRenderContext.mRenderObjects) {
        if (!renderObject.mIsInitialized) {
            renderObject.Initialize();
        }

        renderObject.FixedUpdate(1.f);
    }
}

void DX11RHI::LateUpdate() {
    for (auto& renderObject : mRenderContext.mRenderObjects) {
        if (!renderObject.mIsInitialized) {
            renderObject.Initialize();
        }

        renderObject.LateUpdate(1.f);
    }
}

void DX11RHI::Render() {
    assert(mRenderContext.mDevice);
    assert(mRenderContext.mDeviceContext);

    Clear(FColor(.8f, .8f, .8f, 1.f), 1.f, 0);

    for (FDX11RenderObject& renderObject : mRenderContext.mRenderObjects) {
        renderObject.Render();
    }

    Present();
}

void DX11RHI::Shutdown() {
    mRenderContext.Release();
}

void DX11RHI::Run() {
    Update();

    // Rendering happens in an independent thread from the update methods
    // std::thread render_thread(&DX11RHI::Render, this);
    // render_thread.join();
    Render();
    
    LateUpdate();
}

void DX11RHI::CreateRenderObject() {
 
}

void DX11RHI::Present() {
    assert(mRenderContext.mSwapChain);

    // VSync
    if (false) {
        mRenderContext.mSwapChain->Present(1, 0);
        return;
    }

    mRenderContext.mSwapChain->Present(0, 0);
}

void DX11RHI::Clear(const FColor& clearColor, float clearDepth, uint8 clearStencil) {
    assert(mRenderContext.mDeviceContext);
    mRenderContext.mDeviceContext->ClearRenderTargetView(mRenderContext.mRenderTargetView, Colors::CornflowerBlue);
    mRenderContext.mDeviceContext->ClearDepthStencilView(mRenderContext.mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
}

DXGI_RATIONAL DX11RHI::QueryRefreshRate(uint32 screenWidth, uint32 screenHeight, bool vSync) {
    DXGI_RATIONAL refreshRate = { 0, 1 };
    if (false)
    {
        IDXGIFactory* factory;
        IDXGIAdapter* adapter;
        IDXGIOutput* adapterOutput;
        DXGI_MODE_DESC* displayModeList;

        // Create a DirectX graphics interface factory.
        HRESULT hr = CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)&factory );
        if ( FAILED(hr) )
        {
            MessageBox( 0,
                TEXT("Could not create DXGIFactory instance."),
                TEXT("Query Refresh Rate"),
                MB_OK );

            throw std::exception("Failed to create DXGIFactory.");
        }

        hr = factory->EnumAdapters(0, &adapter );
        if ( FAILED(hr) )
        {
            MessageBox( 0,
                TEXT("Failed to enumerate adapters."),
                TEXT("Query Refresh Rate"),
                MB_OK );

            throw std::exception("Failed to enumerate adapters.");
        }

        hr = adapter->EnumOutputs( 0, &adapterOutput );
        if ( FAILED(hr) )
        {
            MessageBox( 0,
                TEXT("Failed to enumerate adapter outputs."),
                TEXT("Query Refresh Rate"),
                MB_OK );

            throw std::exception("Failed to enumerate adapter outputs.");
        }

        UINT numDisplayModes;
        hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr );
        if ( FAILED(hr) )
        {
            MessageBox( 0,
                TEXT("Failed to query display mode list."),
                TEXT("Query Refresh Rate"),
                MB_OK );

            throw std::exception("Failed to query display mode list.");
        }

        displayModeList = new DXGI_MODE_DESC[numDisplayModes];
        assert( displayModeList);

        hr = adapterOutput->GetDisplayModeList( DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList );
        if ( FAILED(hr) )
        {
            MessageBox( 0,
                TEXT("Failed to query display mode list."),
                TEXT("Query Refresh Rate"),
                MB_OK );

            throw std::exception("Failed to query display mode list.");
        }

        // Now store the refresh rate of the monitor that matches the width and height of the requested screen.
        for ( UINT i = 0; i < numDisplayModes; ++i )
        {
            if ( displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight )
            {
                refreshRate = displayModeList[i].RefreshRate;
            }
        }

        delete [] displayModeList;
        Platform::Windows::SafeRelease(adapterOutput);
        Platform::Windows::SafeRelease(adapter);
        Platform::Windows::SafeRelease(factory);
    }

    return refreshRate;
}