#ifndef ENGINE_SETTINGS_H
#define ENGINE_SETTINGS_H

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
};

#endif // ENGINE_SETTINGS_H
