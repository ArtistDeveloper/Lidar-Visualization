#version 330 core
flat in  int  v_flag;
out vec4 FragColor;

void main()
{
    vec3 col = (v_flag == 1) ? vec3(1.0, 0.2, 0.2)   // 빨강
                             : vec3(1.0);            // 흰색
    FragColor = vec4(col, 1.0);
}