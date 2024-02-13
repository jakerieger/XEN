//
// Created by jr on 2/13/24.
//

#pragma once

#include <stdint.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <glm/vec2.hpp>

#include "STL.h"
#include "Color.h"

namespace TextRenderer {
    struct FCharacter {
        unsigned int TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        unsigned int Advance;
    };

    void Initialize();
    void LoadFont(const char* name, uint32_t size, const char* filename);
    void Shutdown();
    void RenderText(const char* name,
                    const string& text,
                    float posX,
                    float posY,
                    float scale,
                    const FColor& color);

    class AFont {
    public:
        AFont(const char* filename, uint32_t size);
        void LoadCharacters();
        void Cleanup() const;
        FCharacter& GetCharacter(char c);

    private:
        FT_Face m_FontFace {};
        map<char, FCharacter> m_Characters;
    };
}  // namespace TextRenderer