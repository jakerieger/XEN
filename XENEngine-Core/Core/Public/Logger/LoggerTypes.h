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

