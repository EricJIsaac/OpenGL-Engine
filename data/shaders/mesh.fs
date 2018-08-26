#version 330 core

in vec3 norm_ws;

out vec3 color;

void main()
{
	color = norm_ws;
}
