#version 410 core

const float PI = 3.14159265359;

// Input attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

// Output to fragment shader
out vec3 vertexColor;
out vec3 fragNormal;
out vec3 fragPosition;

// Uniforms
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_time;
uniform vec4 u_waveA;
uniform vec4 u_waveB;
uniform vec4 u_waveC;

vec3 GerstnerWave(vec4 wave, vec3 p, inout vec3 tangent, inout vec3 binormal) {
    vec2 dir = normalize(wave.xy);
    float steepness = wave.z;
    float wavelength = wave.w;

    // Wave params
    float k = 2.0 * PI / wavelength;
    float c = sqrt(9.8 / k);
    float f = k * (dot(dir, p.xy) - c * u_time);
    float a = steepness / k;

    // Displacement
    vec3 displacement = vec3(
        dir.x * (a * cos(f)),
        dir.y * (a * cos(f)),
        a * sin(f)
    );

    // Compute tangent
    tangent += vec3(
        -dir.x * dir.x * (steepness * sin(f)),
        dir.x * (steepness * cos(f)),
        -dir.x * dir.y * (steepness * sin(f))
    );

    // Compute binormal
    binormal += vec3(
        -dir.x * dir.y * (steepness * sin(f)),
        dir.y * (steepness * cos(f)),
        -dir.y * dir.y * (steepness * sin(f))
    );

    return displacement;
}

void main() {
    vec3 pos = position;
    vec3 tangent = vec3(1.0, 0.0, 0.0);
    vec3 binormal = vec3(0.0, 0.0, 1.0);

    // Apply waves
    pos += GerstnerWave(u_waveA, position, tangent, binormal);
    pos += GerstnerWave(u_waveB, position, tangent, binormal);
    pos += GerstnerWave(u_waveC, position, tangent, binormal);

    vec3 normal = normalize(cross(tangent, binormal));

    mat3 normMat = transpose(inverse(mat3(u_model)));
    fragNormal = normMat * normal;
    fragPosition = vec3(u_model * vec4(pos, 1.0));
    vertexColor = color;

    gl_Position = u_projection * u_view * vec4(fragPosition, 1.0);
}
