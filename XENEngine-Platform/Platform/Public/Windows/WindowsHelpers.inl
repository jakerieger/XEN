#pragma once
#include <Windows.h>

namespace XEN::Platform::Windows {

// Safely releases a COM object
template<typename T>
inline static void SafeRelease(T& ptr) {
    if (ptr != NULL) {
        ptr->Release();
        ptr = NULL;
    }
}
    
}