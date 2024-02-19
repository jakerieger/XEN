#pragma once

#include "GraphicsContext.h"
#include "Types.h"
#include "STL.h"

#include <result.hpp>
#include <stb_image.h>
#include <GLFW/glfw3.h>

namespace Utilities {
    inline void
    HexToRGBAf(const u32 hex, float& r, float& g, float& b, float& a) {
        const unsigned char alphaByte = (hex >> 24) & 0xFF;
        const unsigned char redByte   = (hex >> 16) & 0xFF;
        const unsigned char greenByte = (hex >> 8) & 0xFF;
        const unsigned char blueByte  = hex & 0xFF;

        a = static_cast<float>(static_cast<u32>(alphaByte) / 255.0);
        r = static_cast<float>(static_cast<u32>(redByte) / 255.0);
        g = static_cast<float>(static_cast<u32>(greenByte) / 255.0);
        b = static_cast<float>(static_cast<u32>(blueByte) / 255.0);
    }

    inline cpp::result<string, int>
    ReadFileToString(const char* path) noexcept {
        if (FILE* fp = fopen(path, "rb")) {
            string contents;
            fseek(fp, 0, SEEK_END);
            contents.resize(ftell(fp));
            rewind(fp);
            fread(&contents[0], 1, contents.size(), fp);
            fclose(fp);
            return contents;
        }
        return cpp::fail(1);
    }

    inline void SetWindowIcon(const char* file) {
        GLFWimage appIcon;
        appIcon.pixels =
          stbi_load(file, &appIcon.width, &appIcon.height, nullptr, 4);
        glfwSetWindowIcon(Graphics::GetWindow(), 1, &appIcon);
        stbi_image_free(appIcon.pixels);
    }
}  // namespace Utilities