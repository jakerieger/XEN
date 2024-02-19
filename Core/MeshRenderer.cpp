//
// Created by conta on 2/18/2024.
//

#include "MeshRenderer.h"
#include "GraphicsContext.h"
#include "Scene.h"
#include "SceneContext.h"

#include <assimp/postprocess.h>

static AMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

AMeshRenderer::AMeshRenderer(const char* path,
                             IMaterial* material,
                             ATransform* transform)
    : m_Material(material), m_Transform(transform) {
    LoadModel(path);
    m_Material->Initialize();
}

void AMeshRenderer::Update(const float deltaTime, FSceneContext& sceneContext) {
    IComponent::Update(deltaTime, sceneContext);
}

void AMeshRenderer::Draw(FSceneContext& sceneContext) {
    ACamera* activeCamera = AScene::GetActiveCamera(sceneContext);

    if (activeCamera) {
        m_Material->GetShader().SetMat4(
          "u_Projection",
          activeCamera->GetProjectionMatrix(Graphics::GetWindowAspect()));
        m_Material->GetShader().SetMat4("u_View",
                                        activeCamera->GetViewMatrix());
        m_Material->GetShader().SetMat4("u_Model",
                                        m_Transform->GetModelMatrix());
        m_Material->GetShader().SetVec3("u_LightColor",
                                        sceneContext.m_Sun.GetColor());
        m_Material->GetShader().SetVec3(
          "u_LightPosition",
          sceneContext.m_Sun.GetTransform().GetPosition());
        m_Material->GetShader().SetVec3(
          "u_ViewPosition",
          activeCamera->GetTransform()->GetPosition());

        m_Material->UpdateUniforms();
    }

    for (auto& mesh : m_Meshes) {
        m_Material->Use();
        mesh.Draw();
    }
}

void AMeshRenderer::Destroyed(FSceneContext& sceneContext) {
    IComponent::Destroyed(sceneContext);

    for (auto& mesh : m_Meshes) {
        mesh.Destroy();
    }
    m_Material->Destroy();
}

void AMeshRenderer::LoadModel(const string& path) {
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

void AMeshRenderer::ProcessNode(const aiNode* node, const aiScene* scene) {
    for (u32 i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (u32 i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

AMesh ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    vector<FVertex> vertices;
    vector<u32> indices;

    for (u32 i = 0; i < mesh->mNumVertices; i++) {
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

    for (u32 i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (u32 j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return AMesh(vertices, indices);
}