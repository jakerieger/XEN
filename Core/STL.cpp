#include "STL.h"

void* operator new[](size_t size,
                     const char* name,
                     int flags,
                     unsigned debugFlags,
                     const char* file,
                     int line) {
    return new uint8_t[size];
}

// I don't fucking know, thanks EA for the documentation ;D
// TODO: figure out what this is supposed to return
void* operator new[](unsigned long,
                     unsigned long,
                     unsigned long,
                     char const*,
                     int,
                     unsigned int,
                     char const*,
                     int) {}