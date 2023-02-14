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

#include "Logger/Logger.h"
#include "Templates/EngineHelpers.inl"

#include <format>
#include <fstream>
#include <bit7z/bitfilecompressor.hpp>

namespace XEN::Core {

static std::map<ELoggerClass, std::string> LogClassMap = {
    { ELoggerClass::LOG_ENGINE, "LogEngine" },
    { ELoggerClass::LOG_RHI, "LogRHI" },
    { ELoggerClass::LOG_EDITOR, "LogEditor" },
    { ELoggerClass::LOG_RUNTIME, "LogRuntime" },
    { ELoggerClass::LOG_PLATFORM, "LogPlatform" }
};
    
static std::map<ELogEntrySeverity, std::string> LogEntrySeverityMap = {
    { ELogEntrySeverity::LES_INFO, "INFO" },
    { ELogEntrySeverity::LES_WARN, "WARNING" },
    { ELogEntrySeverity::LES_ERROR, "ERROR" },
    { ELogEntrySeverity::LES_FATAL, "FATAL" }
};

bool GLogger::mIsLoggingPaused = false;
std::vector<FLogEntry> GLogger::mLogEntries = {};
    
std::string FormatEntryStr(FLogEntry& entry) {
    if (entry.FileName.empty()) {
        return std::format(
            "[{:s}][{:s}] - {:s}: {:s}\n",
            GEngine::GetTimestamp(),
            LogClassMap[entry.LogClass],
            LogEntrySeverityMap[entry.Severity],
            entry.Message
        );
    } else {
        return std::format(
            "[{:s}][{:s}]({:s}: {:d}) - {:s}: {:s}\n",
            GEngine::GetTimestamp(),
            LogClassMap[entry.LogClass],
            entry.FileName,
            entry.FileLine,
            LogEntrySeverityMap[entry.Severity],
            entry.Message
        );
    }
}
    
void GLogger::Log(ELoggerClass LogClass, std::string Message) {
    FLogEntry entry {
        LogClass,
        std::move(Message),
        "",
        0,
        ELogEntrySeverity::LES_INFO
    };

    Log(entry);
}

void GLogger::Log(ELoggerClass LogClass, std::string Message, ELogEntrySeverity Severity) {
    FLogEntry entry {
        LogClass,
        std::move(Message),
        "",
        0,
        Severity
    };

    Log(entry);
}

void GLogger::Log(ELoggerClass LogClass, std::string Message, std::string FileName, unsigned FileLine) {
    FLogEntry entry {
        LogClass,
        std::move(Message),
        std::move(FileName),
        FileLine,
        ELogEntrySeverity::LES_INFO
    };

    Log(entry);
}

void GLogger::Log(
    ELoggerClass LogClass,
    std::string Message,
    std::string FileName,
    unsigned FileLine,
    ELogEntrySeverity Severity
) {
    FLogEntry entry {
        LogClass,
        std::move(Message),
        std::move(FileName),
        FileLine,
        Severity
    };

    Log(entry);
}

void GLogger::Log(FLogEntry& Entry) {
    if (!mIsLoggingPaused) {
        if (!Entry.FileName.empty()) {
            Entry.FileName = StrGetLastInstanceOf(Entry.FileName, '\\');
            mLogEntries.push_back(Entry);
        } else {
            mLogEntries.push_back(Entry);
        }   
    }
}

XEN_RESULT GLogger::CollectLogs(const char* OutDirectory) {
    if (mLogEntries.empty()) { return XEN_FAILED; }
    
    const std::string outFile = std::string(OutDirectory) + "\\" + "XEN_LOG_" + GEngine::GetUnixTimestamp() + ".zip";
    
    // Collect logs in to separate files and zip them
    std::string InfoLogStr;
    std::string WarningLogStr;
    std::string ErrorLogStr;
    std::string FatalLogStr;
    
    for (FLogEntry& entry : mLogEntries) {
        switch (entry.Severity) {
            case ELogEntrySeverity::LES_INFO: {
                InfoLogStr += FormatEntryStr(entry);
                break;
            }
            case ELogEntrySeverity::LES_WARN: {
                WarningLogStr += FormatEntryStr(entry);
                break;
            }
            case ELogEntrySeverity::LES_ERROR: {
                ErrorLogStr += FormatEntryStr(entry);
                break;
            }
            case ELogEntrySeverity::LES_FATAL: {
                FatalLogStr += FormatEntryStr(entry);
                break;
            }
        }
    }

    std::string infoFilePath = std::string(OutDirectory) + "\\Info.txt";
    std::string warningFilePath = std::string(OutDirectory) + "\\Warning.txt";
    std::string errorFilePath = std::string(OutDirectory) + "\\Error.txt";
    std::string fatalFilePath = std::string(OutDirectory) + "\\Fatal.txt";

    std::ofstream infoFile(infoFilePath.c_str());
    std::ofstream warningFile(warningFilePath.c_str());
    std::ofstream errorFile(errorFilePath.c_str());
    std::ofstream fatalFile(fatalFilePath.c_str());

    infoFile << InfoLogStr;
    warningFile << WarningLogStr;
    errorFile << ErrorLogStr;
    fatalFile << FatalLogStr;

    infoFile.close();
    warningFile.close();
    errorFile.close();
    fatalFile.close();

    // Create our log archive
    try {
        using namespace bit7z;

        Bit7zLibrary lib{ "7z.dll" };
        BitFileCompressor compressor{ lib, BitFormat::Zip };

        std::vector<std::string> files = {
            infoFilePath,
            warningFilePath,
            errorFilePath,
            fatalFilePath
        };

        compressor.compress(files, outFile);
        
    } catch (const bit7z::BitException& ex) {
        // handle exception
        printf("bit7z Exception: %s\n", ex.what());
        return XEN_FAILED;
    }

    // Clean up text files
    if (remove(infoFilePath.c_str()) != 0) { return XEN_FAILED; }
    if (remove(warningFilePath.c_str()) != 0) { return XEN_FAILED; }
    if (remove(errorFilePath.c_str()) != 0) { return XEN_FAILED; }
    if (remove(fatalFilePath.c_str()) != 0) { return XEN_FAILED; }

    return XEN_SUCCESS;
}
    
}