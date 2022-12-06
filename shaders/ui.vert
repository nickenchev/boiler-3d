#version 450

#define MAX_OBJECTS 1000

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec4 vertColor;
layout(location = 2) in vec2 vertTexCoord;
layout(location = 3) in vec3 vertNormal;

layout(set = 0, binding = 0) uniform RenderMatrices
{
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	mat4 orphographic;
} rm;

layout(set = 0, binding = 1) uniform Matrices
{
	mat4 data[MAX_OBJECTS];
} matrices;

layout(push_constant) uniform Constants
{
	vec4 offset;
	int matrixId;
	int materialId;
} constants;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec4 fragColor;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragNormal;

void main()
{
	mat4 modelMatrix = matrices.data[constants.matrixId];
	
	vec3 offsetPosition = vertPosition + vec3(constants.offset);
    gl_Position = rm.orphographic * modelMatrix * vec4(offsetPosition, 1.0f);

	fragPosition = vec3(modelMatrix * vec4(offsetPosition, 1.0));
	fragColor = vertColor;
	fragTexCoord = vertTexCoord;
	fragNormal = mat3(transpose(inverse(modelMatrix))) * vertNormal; // TODO: Move the inversion to the CPU
}
