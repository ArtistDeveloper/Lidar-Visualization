#version 330 core
uniform mat4 u_mvp;

layout(location = 0) in vec3  position;
layout(location = 1) in float flag;

flat out int v_flag;                       // fragment 로 전달

void main()
{
    v_flag      = int(flag + 0.5);         // 0.0 → 0, 1.0 → 1
    gl_PointSize = 2.0;
    gl_Position  = u_mvp * vec4(position * 0.02, 1.0);
}