#version 330 core
uniform mat4 u_mvp;
layout(location = 0) in vec3 position;

void main() {
    gl_PointSize = 2.0;
    gl_Position = u_mvp * vec4(position * 0.02, 1.0);
}