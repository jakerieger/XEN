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

#include <vector>

#include "Core.h"
#include "CoreDefines.h"
#include "LoggerTypes.h"
#include "Templates/STLHelpers.inl"

namespace XEN::Core {

/**
 * @brief The GLogger class is the global logging class used by the rest of the engine.
 */
class CORE_API GLogger {
public:
    GLogger() = default;
    
    static void Log(ELoggerClass LogClass, std::string Message);
    static void Log(ELoggerClass LogClass, std::string Message, ELogEntrySeverity Severity);
    static void Log(ELoggerClass LogClass, std::string Message, std::string FileName, unsigned int FileLine);
    static void Log(ELoggerClass LogClass, std::string Message, std::string FileName, unsigned int FileLine, ELogEntrySeverity Severity);
    static void Log(FLogEntry& Entry);

    /**
     * @brief Writes the current log to text files and archives them on disk
     * @param OutDirectory Filesystem path to write the zip archive to
     */
    static XEN_RESULT CollectLogs(const char* OutDirectory);
    
private:
    static bool mIsLoggingPaused;
    static std::vector<FLogEntry> mLogEntries;
};
    
}