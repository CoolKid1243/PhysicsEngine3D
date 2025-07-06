#ifndef SETTINGS_H
#define SETTINGS_H

#include <imgui.h>
#include <glm/glm.hpp>

class Settings {
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

    // Plane settings
    static inline int planeSubdivisions = 160;
    static inline float planeScale = 10.0f;

    // Wave settings
    // Wave A
    inline static glm::vec2 waveDirectionA = glm::normalize(glm::vec2(-0.739f, 0.674f));
    inline static float waveSteepnessA = 0.446f;
    inline static float waveLengthA = 5.281f;
    // Wave B
    inline static glm::vec2 waveDirectionB = glm::normalize(glm::vec2(-0.804f, 0.595f));
    inline static float waveSteepnessB = 0.281f;
    inline static float waveLengthB = 1.582f;
    // Wave C
    inline static glm::vec2 waveDirectionC = glm::normalize(glm::vec2(-0.337f, 0.942f));
    inline static float waveSteepnessC = 0.311f;
    inline static float waveLengthC = 2.838f;

    // Wave shader
    inline static glm::vec3 waterDeepColor = glm::vec3(0.0f, 0.1f, 0.3f);
    inline static glm::vec3 waterShallowColor = glm::vec3(0.3f, 0.7f, 1.0f);
    inline static float ambientStrength = 0.2f;
};

#endif // SETTINGS_H
