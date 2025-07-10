#include "settings.h"
#include "../../engine/engine.h"

static unsigned int vao = 0, vbo = 0, ebo = 0;
static Shader waterShader;
static glm::mat4 model(1.0f);

static int lastSubdivisions = settings::planeSubdivisions;
static float lastScale = settings::planeScale;

void createPlane(unsigned int &vao, unsigned int &vbo, unsigned int &ebo) {
    // Get the scale and number of subdivisions for the plane
    int N = settings::planeSubdivisions;
    float scale = settings::planeScale;

    // Store the planes vertices (position and color) and the indices
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Generate vertex positions and color data
    for (int y = 0; y <= N; ++y) {
        for (int x = 0; x <= N; ++x) {
            float fx = ((float)x / N - 0.5f) * scale;
            float fy = ((float)y / N - 0.5f) * scale;
            // Position
            vertices.push_back(fx);
            vertices.push_back(fy);
            vertices.push_back(0.0f);
            // Color
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

    // Upload vertex/index data to OpenGL
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void updateWaterShaderUniforms(Shader& shader, float time, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    // Set core matrices
    waterShader.setMat4("u_model", model);
    waterShader.setMat4("u_view", view);
    waterShader.setMat4("u_projection", projection);

    // Set time & lighting
    waterShader.setFloat("u_time", (float)glfwGetTime());
    waterShader.setVec3("u_cameraPosition", Camera::Get().position);
    waterShader.setVec3("u_deepColor", settings::waterDeepColor);
    waterShader.setVec3("u_shallowColor", settings::waterShallowColor);
    waterShader.setFloat("u_ambientStrength", settings::ambientStrength);

    // Wave settings
    waterShader.setVec4("u_waveA", glm::vec4(settings::waveDirectionA, settings::waveSteepnessA, settings::waveLengthA));
    waterShader.setVec4("u_waveB", glm::vec4(settings::waveDirectionB, settings::waveSteepnessB, settings::waveLengthB));
    waterShader.setVec4("u_waveC", glm::vec4(settings::waveDirectionC, settings::waveSteepnessC, settings::waveLengthC));
}

static void WaterInit() {
    // Generate the plane
    createPlane(vao, vbo, ebo);

    // Load and compile water shader
    waterShader.init(
        FileManager::read("shaders/wave.vs"),
        FileManager::read("shaders/wave.fs")
    );

    // Rotate the plane (90 degrees) so it lays flat
    model = glm::rotate(glm::mat4(1.0f), -glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));

    // Cache initial settings
    lastSubdivisions = settings::planeSubdivisions;
    lastScale = settings::planeScale;
}

static void WaterUpdate(float deltaTime) {
    // Regenerate the plane mesh if settings changed
    if (settings::planeSubdivisions != lastSubdivisions || settings::planeScale != lastScale) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);

        createPlane(vao, vbo, ebo);
        lastSubdivisions = settings::planeSubdivisions;
        lastScale = settings::planeScale;
    }
}

static void WaterRender() {
    waterShader.use();

    // Get window size for aspect ratio
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

    // Get camera matrices
    glm::mat4 view = Camera::Get().GetViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(engineSettings::fov),
        (float)width / (float)height,
        engineSettings::nearClip,
        engineSettings::farClip
    );

    // Update shader values
    float currentTime = (float)glfwGetTime();
    updateWaterShaderUniforms(waterShader, currentTime, model, view, projection);

    // Draw the mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6 * settings::planeSubdivisions * settings::planeSubdivisions, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void EngineGame::Shutdown() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
