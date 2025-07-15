#include "imgui_window.h"
#include <imgui.h>
#include "../engine_settings.h"
#include "../../sandbox/user/scripts/settings.h"

ImGui_Window::ImGui_Window() {}
ImGui_Window::~ImGui_Window() {}

void Interface() {
    // Settings window
    {
        ImGui::SetNextWindowPos(ImVec2(engineSettings::windowWidth - 820, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);

        if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            if (ImGui::BeginTabBar("Settings")) {
                if (ImGui::BeginTabItem("Display")) {
                    ImGui::Text("Skybox Settings:");
                    ImGui::Checkbox("Wireframe", &engineSettings::wireFrame);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Rendering")) {
                    ImGui::ColorEdit3("Background Color", (float*)&engineSettings::backgroundColor);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Camera")) {
                    ImGui::SliderFloat("FOV", &engineSettings::fov, 15.0f, 165.0f, "%.1f°");
                    ImGui::SliderFloat("Near clip", &engineSettings::nearClip, 0.001f, 1.0f, "%.4f");
                    ImGui::SliderFloat("Far clip", &engineSettings::farClip, 10.0f, 1000.0f, "%.0f");
                    ImGui::SliderFloat("Mouse sensitivity", &engineSettings::mouseSensitivity, 0.01f, 1.0f);
                    ImGui::SliderFloat("Camera speed", &engineSettings::cameraMovementSpeed, 0.1f, 10.0f);
                    ImGui::SliderFloat("Camera accel", &engineSettings::cameraAcceleration, 0.0f, 10.0f);
                    ImGui::SliderFloat("Max camera speed", &engineSettings::maxCameraSpeed, 1.0f, 20.0f);
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }

    // Plane window
    {
        ImGui::SetNextWindowPos(ImVec2(engineSettings::windowWidth - 410, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);

        if (ImGui::Begin("Water Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            if (ImGui::BeginTabBar("Settings")) {
                if (ImGui::BeginTabItem("Mesh")) {
                    ImGui::SliderInt("Subdivisions", &settings::planeSubdivisions, 1.0f, 850.0f);
                    ImGui::SliderFloat("Scale", &settings::planeScale, 0.1f, 45.0f);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Wave Settings")) {
                    ImGui::Text("Wave A:");
                    bool dirAChanged = ImGui::SliderFloat2("Dir A", (float*)&settings::waveDirectionA, -1.0f, 1.0f);
                    if (dirAChanged && glm::length(settings::waveDirectionA) > 0.0001f) {
                        settings::waveDirectionA = glm::normalize(settings::waveDirectionA);
                    }
                    ImGui::SliderFloat("Steep A", &settings::waveSteepnessA, 0.0f, 1.5f);
                    ImGui::SliderFloat("Length A", &settings::waveLengthA, 0.5f, 20.0f);

                    ImGui::Separator();

                    ImGui::Text("Wave B:");
                    bool dirBChanged = ImGui::SliderFloat2("Dir B", (float*)&settings::waveDirectionB, -1.0f, 1.0f);
                    if (dirBChanged && glm::length(settings::waveDirectionB) > 0.0001f) {
                        settings::waveDirectionB = glm::normalize(settings::waveDirectionB);
                    }
                    ImGui::SliderFloat("Steep B", &settings::waveSteepnessB, 0.0f, 1.5f);
                    ImGui::SliderFloat("Length B", &settings::waveLengthB, 0.5f, 20.0f);

                    ImGui::Text("Wave C:");
                    bool dirCChanged = ImGui::SliderFloat2("Dir C", (float*)&settings::waveDirectionC, -1.0f, 1.0f);
                    if (dirCChanged && glm::length(settings::waveDirectionC) > 0.0001f) {
                        settings::waveDirectionC = glm::normalize(settings::waveDirectionC);
                    }
                    ImGui::SliderFloat("Steep C", &settings::waveSteepnessC, 0.0f, 1.5f);
                    ImGui::SliderFloat("Length C", &settings::waveLengthC, 0.5f, 20.0f);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Rendering")) {
                    ImGui::ColorEdit3("Shallow Color", (float*)&settings::waterShallowColor);
                    ImGui::ColorEdit3("Deep Color", (float*)&settings::waterDeepColor);
                    ImGui::SliderFloat("Ambient Strength", &settings::ambientStrength, 0.0f, 1.0f);
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}
