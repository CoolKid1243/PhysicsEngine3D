#ifndef ENGINE_SETTINGS_H
#define ENGINE_SETTINGS_H

#include <glm/glm.hpp>

class engineSettings {
public:
    // Variables
    inline static bool editorMode = false;

    // Window properties
    inline static int windowWidth = 1280;
    inline static int windowHeight = 832;
    inline static const char* windowTitle = "Physics Engine 3D";

    // Display
    inline static bool wireFrame = false;

    // Camera settings
    inline static bool rightMouseHeld = false;
    inline static float fov = 60.0f;
    inline static float nearClip = 0.01f;
    inline static float farClip = 100.0f;
    inline static float mouseSensitivity = 0.1f;
    inline static float cameraMovementSpeed = 3.5f;
    inline static float cameraAcceleration = 6.5f;
    inline static float maxCameraSpeed = 10.0f;

    // Rendering settings
    inline static glm::vec3 backgroundColor = glm::vec3(0.0f, 0.1f, 0.2f);

    // Skybox settings
    inline static glm::vec3 sunDirection = glm::normalize(glm::vec3(-0.2f, 0.7f, 0.3f));
    inline static glm::vec3 sunColor = glm::vec3(1.0f, 0.9f, 0.6f);
    inline static float timeOfDay = 0.5f;
};

#endif // ENGINE_SETTINGS_H
