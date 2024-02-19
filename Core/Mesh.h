//
// Created by jr on 2/14/24.
//
#pragma once

#include "Types.h"
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
    AMesh(const vector<FVertex>& vertices, const vector<u32>& indices)
        : m_VAO(0), m_VBO(0), m_EBO(0), m_Vertices(vertices),
          m_Indices(indices) {
        Initialize();
    }

    void Draw() const;
    void Destroy() const;

    u32 GetNumTriangles() const {
        return static_cast<u32>(m_Indices.size() / 3);
    }

private:
    void Initialize();

    u32 m_VAO;
    u32 m_VBO;
    u32 m_EBO;
    vector<FVertex> m_Vertices;
    vector<u32> m_Indices;
};