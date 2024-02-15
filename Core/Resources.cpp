#include "Resources.h"
#include "STL.h"

#include <filesystem>

namespace Resources {
    string g_Cwd = "";
    namespace fs = std::filesystem;

    void SetCwd(const char* exePath) {
        const string exePathStr = exePath;
#ifdef _WIN32
        const string cwdStr =
          exePathStr.substr(0, exePathStr.find_last_of('\\'));
#else
        const string cwdStr =
          exePathStr.substr(0, exePathStr.find_last_of('/'));
#endif
        g_Cwd = cwdStr;
    }

    /**
     * \brief Gets the full path of a resource stored in the Resources directory
     * \param type Resource type, i.e. RES_FONT, RES_AUDIO, etc
     * \param name File name
     * \return Full path to resource
     */
    string GetResource(const char* type, const char* name) {
        const auto root     = fs::path(g_Cwd.c_str());
        const auto res      = fs::path("Resources");
        const auto subdir   = fs::path(type);
        const auto filename = fs::path(name);
        const auto path     = root / res / subdir / filename;
        if (!exists(path)) {
            fprintf(stderr,
                    "Resource not found: '%s'\n",
                    path.string().c_str());
            throw std::exception();
        }

        return path.string().c_str();
    }
}  // namespace Resources