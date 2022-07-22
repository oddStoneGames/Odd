// Color Square Shader

#type vert
#version 460 core

layout(location = 0)in vec3 a_Pos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0f);
}

#type frag
#version 460 core

out vec4 Color;
uniform vec4 u_Color;

void main() 
{
	Color = u_Color;
}