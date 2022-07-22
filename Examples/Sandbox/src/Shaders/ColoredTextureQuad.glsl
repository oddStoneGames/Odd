// Color Square Shader

#type vert
#version 460 core

layout(location = 0)in vec3 a_Pos;
layout(location = 1)in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 TexCoord;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0f);
	TexCoord = a_TexCoord;
}

#type frag
#version 460 core

in vec2 TexCoord;
out vec4 Color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() 
{
	Color = vec4(texture(u_Texture, TexCoord)) * u_Color;
}