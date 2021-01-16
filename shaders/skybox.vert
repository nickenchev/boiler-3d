#version 450

#define MAX_OBJECTS 1000

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec4 vertColor;
layout(location = 2) in vec3 vertTexCoord;
layout(location = 3) in vec3 vertNormal;

layout(set = 0, binding = 0) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
} vp;

layout(set = 0, binding = 1) uniform Matrices
{
	mat4 data[MAX_OBJECTS];
} matrices;

layout(push_constant) uniform Constants
{
	int matrixId;
	int materialId;
} constants;

layout(location = 0) out vec3 fragPosition;
layout(location = 2) out vec3 fragTexCoord;

void main()
{
	mat4 modelMatrix = matrices.data[constants.matrixId];
	
    gl_Position = vp.projection * vp.view * modelMatrix * vec4(vertPosition, 1.0f);
	fragPosition = vec3(modelMatrix * vec4(vertPosition, 1.0));
	fragTexCoord = vertPosition;
}
