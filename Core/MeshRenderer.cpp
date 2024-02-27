//
// Created by conta on 2/18/2024.
//

#include "MeshRenderer.h"
#include "Engine/GraphicsContext.h"
#include "Scene.h"
#include "SceneContext.h"
#include "Camera.h"
#include "Shader.h"

#include <assimp/postprocess.h>

static AMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

AMeshRenderer::AMeshRenderer(const char* path,
                             eastl::unique_ptr<IMaterial>& material)
    : m_Material(move(material)) {
    LoadModel(path);
    m_Material->Initialize();
    m_DepthShader =
      eastl::unique_ptr<AShader>(new AShader(BuiltinShaders::ShadowDepth));
}

void AMeshRenderer::Start(FSceneContext& sceneContext) {
    IComponent::Start(sceneContext);
}

void AMeshRenderer::Update(const float deltaTime, FSceneContext& sceneContext) {
    IComponent::Update(deltaTime, sceneContext);
}

static glm::mat4 GetLightSpaceMatrix(const float zNear,
                                     const float zFar,
                                     const glm::vec3& lightPosition) {
    const glm::mat4 lightProjection =
      glm::ortho(-10.f, 10.f, -10.f, 10.f, zNear, zFar);
    const glm::mat4 lightView =
      glm::lookAt(lightPosition, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});
    return lightProjection * lightView;
}

// TODO: Find a better way to switch drawing passes without the need
// for game object classes to explicitly supply a flag
void AMeshRenderer::Draw(FSceneContext& sceneContext,
                         const ATransform* transform,
                         const EDrawPass drawPass) {
    ACamera* activeCamera = AScene::GetActiveCamera(sceneContext);

    switch (drawPass) {
        case EDrawPass::PASS_DEPTH: {
            m_DepthShader->Use();
            m_DepthShader->SetMat4("u_Model", transform->GetModelMatrix());

            const auto lightPosition = sceneContext.Sun->GetDirection();
            m_DepthShader->SetMat4(
              "u_LSM",
              GetLightSpaceMatrix(1.f, 7.5f, lightPosition));

            for (auto& mesh : m_Meshes) {
                mesh.Draw();
            }
        } break;
        case EDrawPass::PASS_MAIN: {
            m_Material->Use();
            if (activeCamera) {
                // Update MVP Uniforms (required by all Shaders used by
                // MeshRenderer
                m_Material->GetShader()->SetMat4(
                  "u_Projection",
                  activeCamera->GetProjectionMatrix(
                    Graphics::GetWindowAspect()));
                m_Material->GetShader()->SetMat4("u_View",
                                                 activeCamera->GetViewMatrix());
                m_Material->GetShader()->SetMat4("u_Model",
                                                 transform->GetModelMatrix());

                const auto lightPosition = sceneContext.Sun->GetDirection();
                m_Material->GetShader()->SetMat4(
                  "u_LSM",
                  GetLightSpaceMatrix(1.f, 7.5f, lightPosition));

                // Update material-specific uniforms
                m_Material->UpdateUniforms(sceneContext, activeCamera);
            }

            for (auto& mesh : m_Meshes) {
                mesh.Draw();
            }
        } break;
    }
}

void AMeshRenderer::Destroyed(FSceneContext& sceneContext) {
    IComponent::Destroyed(sceneContext);

    for (auto& mesh : m_Meshes) {
        mesh.Destroy();
    }
    m_Material->Destroy();
}

void AMeshRenderer::LoadModel(const eastl::string& path) {
    Assimp::Importer import;

    constexpr i32 processFlags =
      aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
      aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;

    const aiScene* scene = import.ReadFile(path.c_str(), processFlags);

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
    eastl::vector<FVertex> vertices;
    eastl::vector<u32> indices;

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

        glm::vec3 tangent;
        tangent.x      = mesh->mTangents[i].x;
        tangent.y      = mesh->mTangents[i].y;
        tangent.z      = mesh->mTangents[i].z;
        vertex.Tangent = tangent;

        glm::vec3 bitangent;
        bitangent.x      = mesh->mBitangents[i].x;
        bitangent.y      = mesh->mBitangents[i].y;
        bitangent.z      = mesh->mBitangents[i].z;
        vertex.Bitangent = bitangent;

        if (mesh->mTextureCoords[0]) {
            glm::vec2 tex;
            tex.x           = mesh->mTextureCoords[0][i].x;
            tex.y           = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = tex;
        } else {
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);
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