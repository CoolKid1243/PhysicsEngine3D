#include "camera.h"
#include "../engine_settings.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/normalize_dot.hpp>

Camera::Camera() {
    UpdateVectors();
}
Camera::~Camera() {}

Camera& Camera::Get() {
    static Camera instance; // Only one camera exists
    return instance;
}

void Camera::Rotate(float xOffset, float yOffset) {
    // Add mouse movement to rotation angles
    yaw += xOffset;
    pitch += yOffset;

    // Clamp pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    UpdateVectors();
}

void Camera::Move(bool* keys, float deltaTime, bool isSprinting) {
    // Pick the max speed based of if we are sprinting or base movement speed
    float targetSpeed = isSprinting ? engineSettings::maxCameraSpeed : engineSettings::cameraMovementSpeed;

    // Slowly speed up the camera till it's at it's target speed
    if (speedMultiplier < targetSpeed) {
        speedMultiplier += engineSettings::cameraAcceleration * deltaTime;
    } else {
        speedMultiplier = targetSpeed;
    }

    // Movement direction based of input
    glm::vec3 moveDir(0.0f);
    if (keys[GLFW_KEY_W]) moveDir += direction;
    if (keys[GLFW_KEY_S]) moveDir -= direction;
    if (keys[GLFW_KEY_D]) moveDir += right;
    if (keys[GLFW_KEY_A]) moveDir -= right;
    if (keys[GLFW_KEY_E]) moveDir += up;
    if (keys[GLFW_KEY_Q]) moveDir -= up;

    if (glm::length(moveDir) > 0.01f) {
        moveDir = glm::normalize(moveDir); // Normalize the speed
    }

    position += moveDir * speedMultiplier * deltaTime; // Move the camera
}

void Camera::UpdateVectors() {
    // Convert pitch/yaw to a direction vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(front);

    // Calculate the right and up direction vectors
    right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, direction));
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + direction, up); // Create and return the view matrix
}
