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

#include <string>
#include "Core.h"
#include "CoreTypes.h"

enum CORE_API ELogEntrySeverity {
    LES_INFO,
    LES_WARN,
    LES_ERROR,
    LES_FATAL
};

enum CORE_API ELoggerClass {
    LOG_PLATFORM,
    LOG_RHI,
    LOG_EDITOR,
    LOG_ENGINE,
    LOG_RUNTIME
};

struct CORE_API FLogEntry {
    ELoggerClass LogClass;
    std::string Message;
    std::string FileName;
    uint32 FileLine;
    ELogEntrySeverity Severity;
};

