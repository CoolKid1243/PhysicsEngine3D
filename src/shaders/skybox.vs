#version 410 core
layout(location = 0) in vec3 aPos;

out vec3 fragDirection;

uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    mat4 view = mat4(mat3(u_view));
    vec4 pos = u_projection * view * vec4(aPos, 1.0);

    fragDirection = aPos;
    gl_Position = pos.xyww;
}
