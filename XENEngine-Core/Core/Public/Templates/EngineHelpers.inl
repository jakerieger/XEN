#pragma once

#include <ctime>
#include <chrono>

namespace XEN::Core::GEngine {

inline static std::string GetTimestamp() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%I:%M %p", timeinfo);

    std::string timestamp;
    timestamp += buffer;

    return timestamp;
}

inline static std::string GetUnixTimestamp() {
    const auto p1 = std::chrono::system_clock::now();
    return std::format("{:d}", std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count());
}
    
}