#include "imgui_window.h"
#include <imgui.h>
#include "../settings.h"

ImGui_Window::ImGui_Window() {}
ImGui_Window::~ImGui_Window() {}

void Interface() {
    // Settings window
    {
        ImGui::SetNextWindowPos(ImVec2(Settings::windowWidth - 820, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);

        if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            if (ImGui::BeginTabBar("Settings")) {
                if (ImGui::BeginTabItem("Display")) {
                    ImGui::Checkbox("Wireframe", &Settings::wireFrame);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Rendering")) {
                    ImGui::Text("Rendering settings go here.");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Camera")) {
                    ImGui::SliderFloat("FOV", &Settings::fov, 15.0f, 165.0f, "%.1f°");
                    ImGui::SliderFloat("Near clip", &Settings::nearClip, 0.001f, 1.0f, "%.4f");
                    ImGui::SliderFloat("Far clip", &Settings::farClip, 10.0f, 1000.0f, "%.0f");
                    ImGui::SliderFloat("Mouse sensitivity", &Settings::mouseSensitivity, 0.01f, 1.0f);
                    ImGui::SliderFloat("Camera speed", &Settings::cameraMovementSpeed, 0.1f, 10.0f);
                    ImGui::SliderFloat("Camera accel", &Settings::cameraAcceleration, 0.0f, 10.0f);
                    ImGui::SliderFloat("Max camera speed", &Settings::maxCameraSpeed, 1.0f, 20.0f);
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }

    // Plane window
    {
        ImGui::SetNextWindowPos(ImVec2(Settings::windowWidth - 410, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);

        if (ImGui::Begin("Water Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            if (ImGui::BeginTabBar("Settings")) {
                if (ImGui::BeginTabItem("Mesh")) {
                    ImGui::SliderInt("Subdivisions", &Settings::planeSubdivisions, 1.0f, 850.0f);
                    ImGui::SliderFloat("Scale", &Settings::planeScale, 0.1f, 45.0f);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Wave Settings")) {
                    ImGui::Text("Wave A:");
                    bool dirAChanged = ImGui::SliderFloat2("Dir A", (float*)&Settings::waveDirectionA, -1.0f, 1.0f);
                    if (dirAChanged && glm::length(Settings::waveDirectionA) > 0.0001f) {
                        Settings::waveDirectionA = glm::normalize(Settings::waveDirectionA);
                    }
                    ImGui::SliderFloat("Steep A", &Settings::waveSteepnessA, 0.0f, 1.5f);
                    ImGui::SliderFloat("Length A", &Settings::waveLengthA, 0.5f, 20.0f);

                    ImGui::Separator();

                    ImGui::Text("Wave B:");
                    bool dirBChanged = ImGui::SliderFloat2("Dir B", (float*)&Settings::waveDirectionB, -1.0f, 1.0f);
                    if (dirBChanged && glm::length(Settings::waveDirectionB) > 0.0001f) {
                        Settings::waveDirectionB = glm::normalize(Settings::waveDirectionB);
                    }
                    ImGui::SliderFloat("Steep B", &Settings::waveSteepnessB, 0.0f, 1.5f);
                    ImGui::SliderFloat("Length B", &Settings::waveLengthB, 0.5f, 20.0f);

                    ImGui::Text("Wave C:");
                    bool dirCChanged = ImGui::SliderFloat2("Dir C", (float*)&Settings::waveDirectionC, -1.0f, 1.0f);
                    if (dirCChanged && glm::length(Settings::waveDirectionC) > 0.0001f) {
                        Settings::waveDirectionC = glm::normalize(Settings::waveDirectionC);
                    }
                    ImGui::SliderFloat("Steep C", &Settings::waveSteepnessC, 0.0f, 1.5f);
                    ImGui::SliderFloat("Length C", &Settings::waveLengthC, 0.5f, 20.0f);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Rendering")) {
                    ImGui::ColorEdit3("Shallow Color", (float*)&Settings::waterShallowColor);
                    ImGui::ColorEdit3("Deep Color", (float*)&Settings::waterDeepColor);
                    ImGui::SliderFloat("Ambient Strength", &Settings::ambientStrength, 0.0f, 1.0f);
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}
