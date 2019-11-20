#version 450

layout(location = 0) in vec3 vertPosition;

layout(binding = 0) uniform ModelViewProjection
{
	mat4 model;
	mat4 view;
	mat4 projection;
} mvp;

layout(location = 0) out vec4 outPosition;

void main()
{
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(vertPosition, 1.0f);
	outPosition = gl_Position;
}
