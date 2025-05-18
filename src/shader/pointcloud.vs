#version 330 core
layout(location = 0) in vec3 position;
void main() {
    gl_PointSize = 2.0;
    gl_Position = vec4(position * 0.02, 1.0); // point cloud를 시각화 하기 위해 scaling 테스트
}
