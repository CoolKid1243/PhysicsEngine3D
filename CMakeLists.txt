cmake_minimum_required(VERSION 3.24)
project(PhysicsEngine3D)

# Find libraries
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)

# Engine sources
file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS src/*.h)

# User sources
file(GLOB_RECURSE USER_SOURCES sandbox/user/scripts/*.cpp)
file(GLOB_RECURSE USER_HEADERS sandbox/user/scripts/*.h)

# Executable with main.cpp + all sources
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
    ${USER_SOURCES}
    ${USER_HEADERS}
)

# Copy shaders
file(COPY src/shaders DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY sandbox/user/shaders DESTINATION ${CMAKE_BINARY_DIR}/)

# Link libraries to the target
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} imgui::imgui)
