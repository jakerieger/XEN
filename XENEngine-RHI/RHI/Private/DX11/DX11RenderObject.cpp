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

#include "DX11/DX11RenderObject.h"
#include "DX11/DX11RenderContext.h"
#include "Windows/WindowsHelpers.inl"
#include <cassert>

namespace XEN::RHI {
    
void FDX11RenderObject::Initialize() {
    assert(mRenderContext && "Render Context not valid for Render Object");
    assert(mRenderContext->mDevice && "Device not valid for Render Context");

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.ByteWidth = sizeof(FVertex) * mVertices->size();
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

    resourceData.pSysMem = &mVertices[0];

    HRESULT hr = mRenderContext->mDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &mVertexBuffer);
    assert(SUCCEEDED(hr));

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = sizeof(uint32) * mIndices->size();
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    resourceData.pSysMem = &mIndices[0];

    hr = mRenderContext->mDevice->CreateBuffer(&indexBufferDesc, &resourceData, &mIndexBuffer);
    assert(SUCCEEDED(hr));

    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    hr = mRenderContext->mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffers[CB_APPLICATION]);
    assert(SUCCEEDED(hr));

    hr = mRenderContext->mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffers[CB_FRAME]);
    assert(SUCCEEDED(hr));

    hr = mRenderContext->mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffers[CB_OBJECT]);
    assert(SUCCEEDED(hr));

    ID3DBlob* vertexShaderBlob = nullptr;
    ID3DBlob* pixelShaderBlob = nullptr;

    D3D11_INPUT_ELEMENT_DESC positionDesc = {
        "POSITION",
        0,
        DXGI_FORMAT_R32G32B32_FLOAT,
        0,
        offsetof(FVertex, Position),
        D3D11_INPUT_PER_VERTEX_DATA,
        0
    };

    D3D11_INPUT_ELEMENT_DESC normalsDesc = { 
        "NORMALS",
        0,
        DXGI_FORMAT_R32G32B32_FLOAT,
        0,
        offsetof(FVertex, Normals),
        D3D11_INPUT_PER_VERTEX_DATA,
        0
    };

    D3D11_INPUT_ELEMENT_DESC texCoordDesc = { 
        "TEXCOORDS",
        0,
        DXGI_FORMAT_R32G32_FLOAT,
        0,
        offsetof(FVertex, TexCoords),
        D3D11_INPUT_PER_VERTEX_DATA,
        0
    };

    mVertexLayoutDesc.push_back(positionDesc);
    mVertexLayoutDesc.push_back(normalsDesc);
    mVertexLayoutDesc.push_back(texCoordDesc);

    hr = mRenderContext->mDevice->CreateInputLayout(&mVertexLayoutDesc[0], mVertexLayoutDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &mInputLayout);
    assert(SUCCEEDED(hr));
    
    mRenderContext->mDeviceContext->UpdateSubresource(mConstantBuffers[CB_APPLICATION], 0, nullptr, &mRenderContext->mProjectionMatrix, 0, 0);

    Platform::Windows::SafeRelease(vertexShaderBlob);
    Platform::Windows::SafeRelease(pixelShaderBlob);
    
    mIsInitialized = true;
}

void FDX11RenderObject::Update(float DeltaTime) {
    XMVECTOR eyePosition = XMVectorSet(0, 0, -10, 1);
    XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
    XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
    mRenderContext->mViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
    mRenderContext->mDeviceContext->UpdateSubresource(mConstantBuffers[CB_FRAME], 0, nullptr, &mRenderContext->mViewMatrix, 0, 0);

    mModelMatrix = XMMatrixIdentity();
    mModelMatrix = XMMatrixTranslation(mTransform->Translation.X, mTransform->Translation.Y, mTransform->Translation.Z);
    mModelMatrix = XMMatrixRotationRollPitchYaw(mTransform->Rotation.X, mTransform->Rotation.Y, mTransform->Rotation.Z);
    mModelMatrix = XMMatrixScaling(mTransform->Scale.X, mTransform->Scale.Y, mTransform->Scale.Z);
    mRenderContext->mDeviceContext->UpdateSubresource(mConstantBuffers[CB_OBJECT], 0, nullptr, &mModelMatrix, 0, 0);
}

void FDX11RenderObject::FixedUpdate(float DeltaTime) {
 
}

void FDX11RenderObject::LateUpdate(float DeltaTime) {

}

void FDX11RenderObject::Render() {
    assert(mRenderContext && "Render Context not valid for Render Object");

    const uint32 vertexStride = sizeof(FVertex);
    const uint32 offset = 0;

    mRenderContext->mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &vertexStride, &offset);
    mRenderContext->mDeviceContext->IASetInputLayout(mInputLayout);
    mRenderContext->mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    mRenderContext->mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    mRenderContext->mDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
    mRenderContext->mDeviceContext->VSSetConstantBuffers(0, 3, mConstantBuffers);
    
    mRenderContext->mDeviceContext->RSSetState(mRenderContext->mRasterizerState);
    mRenderContext->mDeviceContext->RSSetViewports(1, &mRenderContext->mViewport);
    
    mRenderContext->mDeviceContext->PSSetShader(mPixelShader, nullptr, 0);
    
    mRenderContext->mDeviceContext->OMSetRenderTargets(1, &mRenderContext->mRenderTargetView, mRenderContext->mDepthStencilView);
    mRenderContext->mDeviceContext->OMSetDepthStencilState(mRenderContext->mDepthStencilState, 1);
    mRenderContext->mDeviceContext->DrawIndexed(mIndices->size(), 0, 0);
}

void FDX11RenderObject::Release() {
    Platform::Windows::SafeRelease(mConstantBuffers[CB_APPLICATION]);
    Platform::Windows::SafeRelease(mConstantBuffers[CB_FRAME]);
    Platform::Windows::SafeRelease(mConstantBuffers[CB_OBJECT]);
    Platform::Windows::SafeRelease(mIndexBuffer);
    Platform::Windows::SafeRelease(mVertexBuffer);
    Platform::Windows::SafeRelease(mInputLayout);
    Platform::Windows::SafeRelease(mVertexShader);
    Platform::Windows::SafeRelease(mPixelShader);
}
 
}