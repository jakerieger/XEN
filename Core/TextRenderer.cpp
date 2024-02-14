//
// Created by jr on 2/13/24.
//

#include "TextRenderer.h"
#include "GraphicsContext.h"
#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

namespace TextRenderer {
    FT_Library g_FontLibrary;
    map<const char*, AFont*> g_Fonts;
    AShader* g_FontShader = nullptr;

    void Initialize() {
        if (FT_Init_FreeType(&g_FontLibrary)) {
            fprintf(stderr, "Failed to initialize FreeType\n");
        }

        g_FontShader = new AShader(BuiltinShaders::Text);
    }

    void LoadFont(const char* name, uint32_t size, const char* filename) {
        const auto font = new AFont(filename, size);
        font->LoadCharacters();
        g_Fonts.insert(pair(name, font));
    }

    void Shutdown() {
        for (auto& [Key, Font] : g_Fonts) {
            Font->Cleanup();
            delete Font;
        }

        FT_Done_FreeType(g_FontLibrary);

        delete g_FontShader;
    }

    void RenderText(const char* name,
                    const string& text,
                    float posX,
                    float posY,
                    float scale,
                    const FColor& color) {
        glm::mat4 projection =
          glm::ortho(0.f,
                     static_cast<float>(Graphics::GetWindowSize().Width),
                     0.f,
                     static_cast<float>(Graphics::GetWindowSize().Height));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(float) * 6 * 4,
                     nullptr,
                     GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              4 * sizeof(float),
                              nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        g_FontShader->Use();
        g_FontShader->SetMat4("projection", projection);
        g_FontShader->SetVec3("textColor", color.Red, color.Green, color.Blue);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all the characters
        AFont* font = g_Fonts[name];
        if (font) {
            for (const char c : text) {
                auto [TextureID, Size, Bearing, Advance] =
                  font->GetCharacter(c);
                const float x = posX + static_cast<float>(Bearing.x) * scale;
                const float y =
                  posY - static_cast<float>(Size.y - Bearing.y) * scale;
                const float w = static_cast<float>(Size.x) * scale;
                const float h = static_cast<float>(Size.y) * scale;

                const float vertices[6][4] = {{x, y + h, 0.0f, 0.0f},
                                              {x, y, 0.0f, 1.0f},
                                              {x + w, y, 1.0f, 1.0f},

                                              {x, y + h, 0.0f, 0.0f},
                                              {x + w, y, 1.0f, 1.0f},
                                              {x + w, y + h, 1.0f, 0.0f}};

                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, TextureID);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph (note that advance is
                // number of 1/64 pixels)
                // bitshift by 6 to get value in pixels (2^6 = 64)
                posX += static_cast<float>((Advance >> 6)) * scale;
            }

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        } else {
            fprintf(stderr, "Could not find font with name: '%s'\n", name);
        }
    }

    AFont::AFont(const char* filename, uint32_t size) {
        if (FT_New_Face(g_FontLibrary, filename, 0, &m_FontFace)) {
            throw std::exception();
        }

        FT_Set_Pixel_Sizes(m_FontFace, 0, size);
    }

    void AFont::LoadCharacters() {
        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++) {
            // load character glyph
            if (FT_Load_Char(m_FontFace, c, FT_LOAD_RENDER)) {
                fprintf(stderr, "Failed to load Glyph: '%c'\n", c);
                continue;
            }

            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RED,
                         static_cast<GLsizei>(m_FontFace->glyph->bitmap.width),
                         static_cast<GLsizei>(m_FontFace->glyph->bitmap.rows),
                         0,
                         GL_RED,
                         GL_UNSIGNED_BYTE,
                         m_FontFace->glyph->bitmap.buffer);

            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // store character for later use
            FCharacter character = {
              texture,
              glm::ivec2(m_FontFace->glyph->bitmap.width,
                         m_FontFace->glyph->bitmap.rows),
              glm::ivec2(m_FontFace->glyph->bitmap_left,
                         m_FontFace->glyph->bitmap_top),
              static_cast<unsigned int>(m_FontFace->glyph->advance.x)};
            m_Characters.insert(pair<char, FCharacter>(c, character));
        }
    }

    void AFont::Cleanup() const {
        FT_Done_Face(m_FontFace);
    }

    FCharacter& AFont::GetCharacter(const char c) {
        return m_Characters[c];
    }
}  // namespace TextRenderer