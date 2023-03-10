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

#include "Platform.h"
#include "RHITypes.h"
#include <functional>

namespace XEN::Platform {

using RHI::ERHI;
    
class PLATFORM_API GenericPlatform {
public:
    virtual void Initialize(ERHI rhi) = 0;
    virtual void Run(std::function<void()> RunHook) = 0;
    virtual void Shutdown() = 0;
    virtual void* GetWindowHandle() = 0;
    
    virtual ~GenericPlatform() = default;
};
  
}