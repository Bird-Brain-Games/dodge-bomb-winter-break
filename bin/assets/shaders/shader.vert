#version 330

uniform mat4 mvp;


layout (location = 0) in vec3 inPosition;
layout (location = 3) in vec2 inCoord;

out vec2 texCoord;

void main()
{
	gl_Position = mvp*vec4(inPosition, 1.0);
	texCoord = inCoord;
}