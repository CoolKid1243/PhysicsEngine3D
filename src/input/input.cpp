#include "input.h"
#include "../engine_settings.h"
#include "../camera/camera.h"

static GLFWwindow* win = nullptr;
static bool firstMouse = true;
static float lastX = 0.0f, lastY = 0.0f;
bool Input::keys[1024]; // Key state array

Input::Input() {}
Input::~Input() {}

void Input::Init(GLFWwindow *window) {
    win = window;

    // Set GLFW callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);

    // Hide and lock the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Keep track if a key is pressed or released
    keys[key] = (action != GLFW_RELEASE);

    // Commmand + I toggles editor mode
    if (key == GLFW_KEY_I && action == GLFW_PRESS && (mods & GLFW_MOD_SUPER)) {
        engineSettings::editorMode = !engineSettings::editorMode;

        // Show or hide the mouse based on if we are in editor mode
        if (engineSettings::editorMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }
    }

    // Control + I toggles editor mode
    if (key == GLFW_KEY_I && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)) {
        engineSettings::editorMode = !engineSettings::editorMode;

        // Show or hide the mouse based on if we are in editor mode
        if (engineSettings::editorMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }
    }
}

void Input::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    // Only allow us to rotate the camera if we are holding right-click
    if (engineSettings::editorMode && button == GLFW_MOUSE_BUTTON_RIGHT) {
        engineSettings::rightMouseHeld = (action == GLFW_PRESS);

        // Reset the mouse movement
        if (engineSettings::rightMouseHeld) {
            firstMouse = true;
        }
    }
}

void Input::CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    // Onlt rotate the camera if not in editor or if we are in editor holding right-click
    if (!engineSettings::editorMode || (engineSettings::editorMode && engineSettings::rightMouseHeld)) {
        if (firstMouse) {
            lastX = (float)xpos;
            lastY = (float)ypos;
            firstMouse = false;
        }

        // Calculate how far the mouse if moved
        float xoffset = (float)xpos - lastX;
        float yoffset = lastY - (float)ypos;

        // Save the new mouse position
        lastX = (float)xpos;
        lastY = (float)ypos;

        // Rotate the camera times the mouse sensitivity
        xoffset *= engineSettings::mouseSensitivity;
        yoffset *= engineSettings::mouseSensitivity;

        // Rotate camera
        Camera::Get().Rotate(xoffset, yoffset);
    }
}

void Input::Update(float deltaTime) {
    // Use shift to sprint
    bool sprint = keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT];

    // Move camera with keys
    Camera::Get().Move(keys, deltaTime, sprint);
}
