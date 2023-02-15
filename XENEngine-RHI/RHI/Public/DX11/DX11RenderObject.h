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

#include "CoreTypes.h"
#include "RHI.h"
#include "DX11Includes.h"
#include "Math/MathTypes.h"
#include <vector>

namespace XEN::RHI {

using namespace DirectX;
using namespace XEN::Core::FMath;

struct RHI_API FDX11RenderContext;

struct RHI_API FDX11RenderObject {
    ID3D11InputLayout* mInputLayout = nullptr;
    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11Buffer* mIndexBuffer = nullptr;
    std::vector<D3D11_INPUT_ELEMENT_DESC> mVertexLayoutDesc = {};
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11PixelShader* mPixelShader = nullptr;
    ID3D11Buffer* mConstantBuffers[EConstantBuffer::NUM_CONSTANT_BUFFERS] = {};
    XMMATRIX mModelMatrix = {};
    FTransform* mTransform;
    FDX11RenderContext* mRenderContext = nullptr;

    void Initialize();
    void Update(float DeltaTime);
    void FixedUpdate(float DeltaTime);
    void LateUpdate(float DeltaTime);
    void Render();
    void Release();

    bool mIsInitialized = false;
    std::vector<FVertex>* mVertices = nullptr;
    std::vector<uint32>* mIndices = nullptr;
};
 
}