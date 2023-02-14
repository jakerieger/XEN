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