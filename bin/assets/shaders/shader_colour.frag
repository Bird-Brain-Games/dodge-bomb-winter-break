#version 330

in vec3 colour;
out vec4 outputColor;

void main()
{
	outputColor = vec4(colour, 1.0);
}