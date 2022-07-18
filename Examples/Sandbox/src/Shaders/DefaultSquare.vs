#version 460 core

layout(location = 0)in vec3 pos;
layout(location = 1)in vec2 texCoord;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec2 TexCoord;

void main() 
{
	gl_Position = viewProjection * transform * vec4(pos, 1.0f);
	TexCoord = texCoord;
}