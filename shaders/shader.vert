#version 450

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec4 vertColour;
layout(location = 2) in vec2 vertTexCoord;

layout(binding = 0) uniform ModelViewProjection
{
	mat4 model;
	mat4 view;
	mat4 projection;
} mvp;

layout(location = 1) out vec4 fragColour;
layout(location = 2) out vec2 fragTexCoord;

void main()
{
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(vertPosition, 1.0f);
	fragColour = vertColour;
	fragTexCoord = vertTexCoord;
}
