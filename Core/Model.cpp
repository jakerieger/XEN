//
// Created by jr on 2/15/24.
//

#include "Model.h"

#include "Camera.h"
#include "GraphicsContext.h"
#include "SceneContext.h"

#include <assimp/postprocess.h>

AModel::AModel(const char* path, IMaterial* material) : m_Material(material) {
    LoadModel(path);
    m_Material->Initialize();
}

void AModel::Update(float deltaTime) {
    m_Transform.Update();
    m_Material->Use();
    m_Material->GetShader().SetMat4("u_Model", m_Transform.GetModelMatrix());
}

void AModel::Draw(FSceneContext& sceneContext) {
    ACamera* activeCamera = nullptr;
    for (auto& cam : sceneContext.m_Cameras) {
        if (cam.GetActive()) {
            activeCamera = &cam;
        }
    }

    if (activeCamera) {
        m_Material->GetShader().SetMat4(
          "u_Projection",
          activeCamera->GetProjectionMatrix(45.f, Graphics::GetWindowAspect()));
        m_Material->GetShader().SetMat4("u_View",
                                        activeCamera->GetViewMatrix());
        m_Material->GetShader().SetMat4("u_Model",
                                        m_Transform.GetModelMatrix());
        m_Material->GetShader().SetVec3("u_LightColor",
                                        sceneContext.m_Sun.GetColor());
        m_Material->GetShader().SetVec3(
          "u_LightPosition",
          sceneContext.m_Sun.GetTransform().GetPosition());
        m_Material->GetShader().SetVec3("u_ViewPosition",
                                        activeCamera->GetPosition());

        m_Material->UpdateUniforms();
    }

    for (auto& mesh : m_Meshes) {
        m_Material->Use();
        mesh.Draw();
    }
}

void AModel::Destroy() {
    for (auto& mesh : m_Meshes) {
        mesh.Destroy();
    }
    m_Material->Destroy();
}

void AModel::LoadModel(const string& path) {
    Assimp::Importer import;
    const aiScene* scene =
      import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        fprintf(stderr, "ERROR::ASSIMP::%s\n", import.GetErrorString());
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void AModel::ProcessNode(const aiNode* node, const aiScene* scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

AMesh AModel::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    vector<FVertex> vertices;
    vector<uint32_t> indices;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        FVertex vertex;

        glm::vec3 position;
        position.x      = mesh->mVertices[i].x;
        position.y      = mesh->mVertices[i].y;
        position.z      = mesh->mVertices[i].z;
        vertex.Position = position;

        glm::vec3 normal;
        normal.x      = mesh->mNormals[i].x;
        normal.y      = mesh->mNormals[i].y;
        normal.z      = mesh->mNormals[i].z;
        vertex.Normal = normal;

        if (mesh->mTextureCoords[0]) {
            glm::vec2 tex;
            tex.x            = mesh->mTextureCoords[0][i].x;
            tex.y            = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = tex;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return AMesh(vertices, indices);
}