#pragma once

#include <memory>
#include <vector>
#include <map>

#include "Core.h"
#include "CoreDefines.h"
#include "LoggerTypes.h"
#include "Templates/STLHelpers.inl"

namespace XEN::Core {

class CORE_API GLogger {
public:
    GLogger() = default;
    
    static void Log(ELoggerClass LogClass, std::string Message);
    static void Log(ELoggerClass LogClass, std::string Message, ELogEntrySeverity Severity);
    static void Log(ELoggerClass LogClass, std::string Message, std::string FileName, unsigned int FileLine);
    static void Log(ELoggerClass LogClass, std::string Message, std::string FileName, unsigned int FileLine, ELogEntrySeverity Severity);
    static void Log(FLogEntry& Entry);

    static XEN_RESULT CollectLogs(const char* OutDirectory);
    
private:
    static bool mIsLoggingPaused;
    static std::vector<FLogEntry> mLogEntries;
};
    
}