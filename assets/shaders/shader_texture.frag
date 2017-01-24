#version 330

in vec2 UV;
in vec3 normal0;
out vec4 outputColor;

uniform sampler2D gSampler;

void main()
{
	outputColor = texture2D(gSampler, UV);
}