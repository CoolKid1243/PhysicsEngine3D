#version 410 core

// Input attributes
in vec3 fragNormal;
in vec3 fragPosition;

// Output to vertex shader
out vec4 FragColor;

// Uniforms
uniform vec3 u_cameraPosition;
uniform vec3 u_deepColor;
uniform vec3 u_shallowColor;
uniform float u_ambientStrength;

// Water colors
const vec3 deepWaterColor = vec3(0.0, 0.1, 0.3);
const vec3 shallowWaterColor = vec3(0.3, 0.7, 1.0);

const vec3 lightDir = normalize(vec3(0.5, 1.0, 0.2));
const vec3 lightColor = vec3(1.0, 0.95, 0.9);

void main() {
    vec3 N = normalize(fragNormal); // Surface normal
    vec3 V = normalize(u_cameraPosition - fragPosition); // View direction

    // Lighting
    float diffuse = max(dot(N, lightDir), 0.0);
    vec3 H = normalize(lightDir + V);
    float spec = pow(max(dot(N, H), 0.0), 64.0);
    float fresnel = pow(1.0 - max(dot(N, V), 0.0), 3.0) * 0.3;

    // Base color depending on angle of surface
    float facingUp = clamp(N.z, 0.0, 1.0);
    vec3 waterColor = mix(u_deepColor, u_shallowColor, facingUp);

    // Add ambient, diffuse, and specular + fresnel reflection
    vec3 lighting = waterColor * (u_ambientStrength + diffuse) + lightColor * spec * fresnel;

    FragColor = vec4(lighting, 1.0);
}
