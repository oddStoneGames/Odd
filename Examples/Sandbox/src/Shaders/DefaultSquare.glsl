// Basic Square Shader

#type vert
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

#type frag
#version 460 core

in vec2 TexCoord;
out vec4 Color;

uniform float intensity;

uniform sampler2D baseColorTexture;

void main() 
{
	vec4 col = texture(baseColorTexture, TexCoord);
	Color = col * intensity;
}