#version 450

layout(location = 0) in vec3 vertPosition;

layout(location = 0) out vec4 outPosition;

void main()
{
    gl_Position = vec4(vertPosition, 1.0f);
	outPosition = gl_Position;
}
