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

#include <type_traits>
#include <sstream>
#include <string>

#define CHECK(hr) ()

// Template helper for mapping Enums to Strings
// Based on implementation here: https://stackoverflow.com/a/208003
template<typename T> struct MapInitHelper {
    T& data;
    MapInitHelper(T& d) : data(d) {}
    MapInitHelper& operator() (typename T::key_type const& key, typename T::mapped_type const& value) {
        data[key] = value;
        return *this;
    }
};

template<typename T> MapInitHelper<T> MapInit(T& item) {
    return MapInitHelper<T>(item);
}

template<typename T> T* Cast(T* ChildClass) {
    return dynamic_cast<T>(ChildClass);
}

template<typename T1, typename T2> bool SubclassOf() {
    return std::is_base_of<T1, T2>::value;
}

inline std::string StrGetLastInstanceOf(std::string const& String, char Char) {
    std::stringstream stream(String);
    std::string segment;
    std::vector<std::string> segments;

    while(std::getline(stream, segment, Char)) {
        segments.push_back(segment);
    }
    
    return segments.back();
}