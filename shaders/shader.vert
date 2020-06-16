#version 450

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec4 vertColour;
layout(location = 2) in vec2 vertTexCoord;
layout(location = 3) in vec3 vertNormal;

layout(binding = 0) uniform ModelViewProjection
{
	mat4 model;
	mat4 view;
	mat4 projection;
} mvp;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec4 fragColour;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragNormal;

void main()
{
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(vertPosition, 1.0f);
	fragPosition = vec3(mvp.model * vec4(vertPosition, 1.0));
	fragColour = vertColour;
	fragTexCoord = vertTexCoord;
	fragNormal = vertNormal;
}
