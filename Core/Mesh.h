//
// Created by jr on 2/14/24.
//
#pragma once

#include <stdint.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "STL.h"

#include <Shader.h>

struct FVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

/**
 * \brief Barebones representation of a renderable object
 */
class AMesh {
public:
    AMesh(const vector<FVertex>& vertices, const vector<uint32_t>& indices)
        : m_VAO(0), m_VBO(0), m_EBO(0), m_Vertices(vertices),
          m_Indices(indices) {
        Initialize();
    }

    void Draw() const;
    void Destroy() const;

private:
    void Initialize();

    uint32_t m_VAO;
    uint32_t m_VBO;
    uint32_t m_EBO;
    vector<FVertex> m_Vertices;
    vector<uint32_t> m_Indices;
};