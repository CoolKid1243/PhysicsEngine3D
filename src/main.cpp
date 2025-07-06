#include <glm/fwd.hpp>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include "extern/imgui_impl_glfw.h"
#include "extern/imgui_impl_opengl3.h"

#include "settings.h"
#include "file_manager/file_manager.h"
#include "shader/shader.h"
#include "camera/camera.h"
#include "input/input.h"
#include "imgui/imgui_window.h"

void errorCallback(int error, const char *description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void updateShaderUniforms(Shader& shader, float currentTime, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    shader.setMat4("u_model", model);
    shader.setMat4("u_view", view);
    shader.setMat4("u_projection", projection);
    shader.setFloat("u_time", currentTime);
    shader.setVec3("u_cameraPosition", Camera::Get().position);
    shader.setVec3("u_deepColor", Settings::waterDeepColor);
    shader.setVec3("u_shallowColor", Settings::waterShallowColor);
    shader.setFloat("u_ambientStrength", Settings::ambientStrength);

    // Wave uniforms
    shader.setVec4("u_waveA", glm::vec4(
        Settings::waveDirectionA.x,
        Settings::waveDirectionA.y,
        Settings::waveSteepnessA,
        Settings::waveLengthA
    ));
    shader.setVec4("u_waveB", glm::vec4(
        Settings::waveDirectionB.x,
        Settings::waveDirectionB.y,
        Settings::waveSteepnessB,
        Settings::waveLengthB
    ));
    shader.setVec4("u_waveC", glm::vec4(
        Settings::waveDirectionC.x,
        Settings::waveDirectionC.y,
        Settings::waveSteepnessC,
        Settings::waveLengthC
    ));
}

void createPlane(unsigned int &vao, unsigned int &vbo, unsigned int &ebo) {
    // Get the scale and number of subdivisions for the plane
    int N = Settings::planeSubdivisions;
    float scale = Settings::planeScale;

    // Store the planes vertices (position and color) and the indices
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Generate vertices
    for (int y = 0; y <= N; ++y) {
        for (int x = 0; x <= N; ++x) {
            float fx = ((float)x / N - 0.5f) * scale;
            float fy = ((float)y / N - 0.5f) * scale;
            // Position
            vertices.push_back(fx);
            vertices.push_back(fy);
            vertices.push_back(0.0f);
            // Color (simple gradient)
            vertices.push_back((float)x / N);
            vertices.push_back((float)y / N);
            vertices.push_back(1.0f - (float)x / N);
        }
    }

    // Generate indices
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x) {
            int i = y * (N + 1) + x;
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + N + 1);

            indices.push_back(i + 1);
            indices.push_back(i + N + 2);
            indices.push_back(i + N + 1);
        }
    }

    // Generate VAO, VBO, EBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Use VAO
    glBindVertexArray(vao);

    // Use VBO and send vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Use EBO and send index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set position data (attribute 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Set color data (attribute 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind VAO
    glBindVertexArray(0);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW error callback
    glfwSetErrorCallback(errorCallback);

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // OpenGL 4.1
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac

    // Create window
    GLFWwindow *window = glfwCreateWindow(Settings::windowWidth, Settings::windowHeight, Settings::windowTitle, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Setup input and hide cursor
    Input::Init(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Make OpenGL context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);    

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    // Create triangle
    unsigned int vao, vbo, ebo;
    createPlane(vao, vbo, ebo);

    // Initialize shader
    Shader simpleShader;
    simpleShader.init(
        FileManager::read("shaders/simple.vs"),
        FileManager::read("shaders/simple.fs")
    );

    // Get screen dimensions
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    // Camera setup
    glm::vec3 cameraPosition(0.f, 0.f, 2.f);
    glm::vec3 cameraViewDirection(0.f, 0.f, -1.f);

    // Model matrix
    glm::mat4 model(1.f);

    // View matrix
    glm::mat4 view = glm::lookAt(
        cameraPosition,
        cameraPosition + cameraViewDirection, // Target position (camera looks here)
        glm::vec3(0.f, 1.f, 0.f) // Up vector
    );

    // Projection matrix
    glm::mat4 projection = glm::perspective(
        glm::radians(Settings::fov), // Field of view
        (float)screenWidth / (float)screenHeight, // Aspect ratio (width / height)
        Settings::nearClip, // Near clipping plane
        Settings::farClip // Far clipping plane
    );

    // Rotate the plane -90 degrees to lay flat
    model = glm::rotate(model, -glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f));

    // Get the last value of the planes subdivision and scale
    int lastSubdivisions = Settings::planeSubdivisions;
    float lastScale = Settings::planeScale;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Get delta time
        float currentTime = (float)glfwGetTime();
        static float lastFrame = 0.0f;
        float deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        // Update the input system
        Input::Update(deltaTime);

        // Enable or disable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, Settings::wireFrame ? GL_LINE : GL_FILL);

        // Clear color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f); // Background color

        if (Settings::planeSubdivisions != lastSubdivisions || Settings::planeScale != lastScale) {
            // Cleanup old buffers
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ebo);
            // Recreate plane
            createPlane(vao, vbo, ebo);
            lastSubdivisions = Settings::planeSubdivisions;
            lastScale = Settings::planeScale;
        }

        // Update view matrix
        view = Camera::Get().GetViewMatrix();

        // Render the plane
        {
            // Activate the shader to use it for rendering
            simpleShader.use();

            // Bind the VAO that stores the plane's vertex data and settings
            glBindVertexArray(vao);

            // Issue the draw command
            glDrawElements(
                GL_TRIANGLES,
                6 * Settings::planeSubdivisions * Settings::planeSubdivisions, // total indices
                GL_UNSIGNED_INT,
                0
            );

            // Unbind the VAO
            glBindVertexArray(0);
        }

        if (Settings::editorMode) {
            // Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Create ImGui window
            Interface();

            // Render ImGui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        // Update the shader uniforms
        simpleShader.use();
        updateShaderUniforms(simpleShader, currentTime, model, view, projection);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup plane
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
