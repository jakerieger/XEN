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

#include "CoreDefines.h"
#include "CoreMinimal.h"
#include <cstdio>

XEN_USE_CORE_NAMESPACE

int main(int argc, char* argv[]) {
    GLogger::Log(ELoggerClass::LOG_ENGINE, "Test log");
    GLogger::CollectLogs(R"(C:\Users\Jake\Desktop)");
    
    return 0;
}
