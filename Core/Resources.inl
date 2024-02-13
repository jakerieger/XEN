#pragma once

#include "STL.h"

namespace Resources {
    inline string g_Cwd = "";

    inline void SetCwd(const char* exePath) {
        const string exePathStr = exePath;
        const string cwdStr =
          exePathStr.substr(0, exePathStr.find_last_of('/'));
        g_Cwd = cwdStr;
    }

}  // namespace Resources