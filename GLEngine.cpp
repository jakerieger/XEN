#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <memory>
#include <result.hpp>
#include <sstream>
#include <stdio.h>

#include "Classes/Shader.h"
#include "stb_image.h"

namespace GL {
    bool g_IsWireframe = false;

    auto OnFramebufferResize(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    auto OnKeyPress(
      GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
            if (g_IsWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }

            g_IsWireframe = !g_IsWireframe;
        }
    }

    struct DestroyGLFWWindow {
        auto operator()(GLFWwindow* ptr) const noexcept {
            glfwDestroyWindow(ptr);
        }
    };

    using GLWindow = std::unique_ptr<GLFWwindow, DestroyGLFWWindow>;

    enum class GLError {
        GL_ERROR_INIT,
        GL_ERROR_CREATE_WINDOW,
        GL_ERROR_GLAD,
    };
}  // namespace GL

using GL::GLError;
using GL::GLWindow;

struct Size {
    uint16_t Width;
    uint16_t Height;
};

namespace Engine {
    GLWindow g_Window;
    Size g_WindowSize         = {1600, 900};
    const char* g_WindowTitle = "GLEngine";

    auto GetWindow() -> GLFWwindow* {
        return g_Window.get();
    }
}  // namespace Engine

namespace Utils {
    auto hexToRGBf(uint32_t hex, float& r, float& g, float& b) {
        unsigned char redByte   = (hex >> 16) & 0xFF;
        unsigned char greenByte = (hex >> 8) & 0xFF;
        unsigned char blueByte  = hex & 0xFF;

        r = (uint32_t)redByte / 255.f;
        g = (uint32_t)greenByte / 255.f;
        b = (uint32_t)blueByte / 255.f;
    }

    auto LoadShaderFromFile(const char* path) -> std::string {
        std::ifstream f(path);
        std::stringstream buffer;
        buffer << f.rdbuf();
        f.close();
        return buffer.str();
    }

    auto LoadTexture(const char* name,
                     int& width,
                     int& height,
                     int& numChannels,
                     uint32_t& outTexture) noexcept {
        unsigned char* data = stbi_load(name, &width, &height, &numChannels, 0);

        // OpenGL stuff
        if (data) {
            glGenTextures(1, &outTexture);
            glBindTexture(GL_TEXTURE_2D, outTexture);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         width,
                         height,
                         0,
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        stbi_image_free(data);
    }
}  // namespace Utils

namespace TestData {
    float vertices[] = {
      0.5f,
      0.5f,
      0.0f,  // top right
      0.5f,
      -0.5f,
      0.0f,  // bottom right
      -0.5f,
      -0.5f,
      0.0f,  // bottom left
      -0.5f,
      0.5f,
      0.0f  // top left
    };
    uint32_t indices[] = {0, 1, 3, 1, 2, 3};
    float texCoords[]  = {
      0.0f,
      0.0f,  // lower-left corner
      1.0f,
      0.0f,  // lower-right corner
      0.5f,
      1.0f  // top-center corner
    };
    uint32_t VBO;
    uint32_t VAO;
    uint32_t EBO;
    AShader* shader;

    auto InitTestData() {
        shader = new AShader("Resources/Shaders/triangle.glsl");

        // Generate OpenGL Buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the Vertex Array Object first, then bind and set the vertex
        // buffer(s), and finally configure vertex attributes
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertices),
                     vertices,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(indices),
                     indices,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              3 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);

        // Note that this is allowed, the call to glVertexAttribPointer
        // registered VBO as the vertex attribute's bound vertex buffer object
        // so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound
        // element buffer object IS stored in the VAO; keep the EBO bound.
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't
        // accidentally modify this VAO, but this rarely happens. Modifying
        // other VAOs requires a call to glBindVertexArray anyways so we
        // generally don't unbind VAOs (nor VBOs) when it's not directly
        // necessary.
        glBindVertexArray(0);

        // Polygon mode (for wireframe)
        // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }

    auto RenderTestData() {
        shader->Use();

        float time  = glfwGetTime();
        float green = (sin(time) / 2.0f) + 0.5f;
        shader->SetVec4("ourColor", 1.f, green, 0.f, 2.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    auto CleanupTestData() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        shader->Destroy();
        delete shader;
    }
}  // namespace TestData

auto CreateWindow() noexcept -> int {
    Engine::g_Window = GLWindow(glfwCreateWindow(Engine::g_WindowSize.Width,
                                                 Engine::g_WindowSize.Height,
                                                 Engine::g_WindowTitle,
                                                 nullptr,
                                                 nullptr));
    if (Engine::GetWindow() == nullptr) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(Engine::GetWindow());
    return 0;
}

auto InitOpenGL() noexcept -> cpp::result<void, GLError> {
    if (glfwInit() != GLFW_TRUE) {
        return cpp::fail(GLError::GL_ERROR_INIT);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (const auto result = CreateWindow(); result != 0) {
        return cpp::fail(GLError::GL_ERROR_CREATE_WINDOW);
    }

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        fprintf(stderr, "Failed to initialize OpenGL\n");
        return cpp::fail(GLError::GL_ERROR_GLAD);
    }

    glViewport(0, 0, Engine::g_WindowSize.Width, Engine::g_WindowSize.Height);
    glfwSetFramebufferSizeCallback(Engine::GetWindow(),
                                   GL::OnFramebufferResize);
    glfwSetKeyCallback(Engine::GetWindow(), GL::OnKeyPress);

    return {};
}

auto Render() noexcept {
    // Main render/game loop
    float r, g, b;
    Utils::hexToRGBf(0x000000, r, g, b);

    while (!glfwWindowShouldClose(Engine::GetWindow())) {
        //==================== RENDER COMMANDS ====================//
        glClearColor(r, g, b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        TestData::RenderTestData();
        //=========================================================//

        glfwSwapBuffers(Engine::GetWindow());
        glfwPollEvents();
    }
}

auto Shutdown() noexcept {
    TestData::CleanupTestData();
    glfwTerminate();
}

auto main(int argc, char* argv[]) -> int {
    InitOpenGL().expect("OpenGL failed to initialize");
    TestData::InitTestData();
    Render();
    Shutdown();

    return 0;
}
