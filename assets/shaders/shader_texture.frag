#version 330

in vec2 UV;
out vec4 outputColor;

uniform sampler2D gSampler;

void main()
{
	outputColor = texture2D(gSampler, UV);
}