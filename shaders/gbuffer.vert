#version 450

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec4 vertColor;
layout(location = 2) in vec2 vertTexCoord;
layout(location = 3) in vec3 vertNormal;

layout(binding = 0) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
} vp;

layout(binding = 1) uniform Matrices
{
	mat4 list[1000];
} matrices;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec4 fragColor;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragNormal;

void main()
{
	mat4 modelMatrix = matrices.list[0]; // CHANGE THIS!!!
	
    gl_Position = vp.projection * vp.view * modelMatrix * vec4(vertPosition, 1.0f);
	fragPosition = vec3(modelMatrix * vec4(vertPosition, 1.0));
	fragColor = vertColor;
	fragTexCoord = vertTexCoord;
	fragNormal = mat3(transpose(inverse(modelMatrix))) * vertNormal; // TODO: Move the inversion to the CPU
}
