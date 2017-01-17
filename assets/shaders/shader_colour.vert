#version 330

uniform mat4 mvp;


layout (location = 0) in vec3 inPosition;

out vec3 colour;

void main()
{
	gl_Position = mvp*vec4(inPosition, 1.0);
	colour = vec3(1.0, 0.0, 0.0);
}