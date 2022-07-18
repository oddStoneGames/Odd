#version 460 core

in vec2 TexCoord;
out vec4 Color;

uniform float intensity;

uniform sampler2D baseColorTexture;

void main() 
{
	Color = vec4(texture(baseColorTexture, TexCoord).rgb * intensity, 1.0f);
}