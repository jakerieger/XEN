#include "STL.h"

void* operator new[](size_t size,
                     const char* name,
                     int flags,
                     unsigned debugFlags,
                     const char* file,
                     int line) {
    return new uint8_t[size];
}

#ifdef _WIN32
// I don't fucking know, thanks EA for the documentation ;D
// Update: this works, I don't know why
void* operator new[](size_t size,
                     unsigned long long,
                     unsigned long long,
                     char const*,
                     int,
                     unsigned int,
                     char const*,
                     int) {
    return new uint8_t[size];
}
#else
void* operator new[](size_t size, unsigned long, unsigned long, char const*, int, unsigned int, char const*, int) {
    return new uint8_t[size];
}
#endif