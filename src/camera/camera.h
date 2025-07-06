#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    ~Camera();

    static Camera& Get(); // Creates a simgle camera instance

    void Rotate(float xOffset, float yOffset);
    void Move(bool* keys, float deltaTime, bool isSprinting);
    glm::mat4 GetViewMatrix();

    glm::vec3 position = glm::vec3(0.f, 2.f, 3.f);
    float pitch = 0.0f;
    float yaw = -90.0f; // Looking toward -Z

private:
    glm::vec3 direction, right, up;
    float speedMultiplier = 1.0f;

    void UpdateVectors();
};

#endif // CAMERA_H
