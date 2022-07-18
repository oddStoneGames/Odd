#version 460 core

in vec2 TexCoord;
out vec4 Color;

uniform sampler2D baseColorTexture;

void main() 
{
	Color = vec4(texture(baseColorTexture, TexCoord).rgb, 1.0f);
} 