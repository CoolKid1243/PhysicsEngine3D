#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

class Input {
public:
    Input();
    ~Input();

    static void Init(GLFWwindow* window);
    static void Update(float deltaTime);
    static bool keys[1024]; // Stores wich keys are held down

private:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif // INPUT_H
