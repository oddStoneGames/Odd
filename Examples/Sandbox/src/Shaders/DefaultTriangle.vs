#version 460 core

layout(location = 0)in vec3 pos;
layout(location = 1)in vec3 color;

uniform mat4 viewProjection;

out vec3 FragColor;
void main() 
{
    gl_Position = viewProjection * vec4(pos, 1.0f);
    FragColor = color;
}