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
	mat4 orphographic;
} vp;

layout(set = 0, binding = 1) uniform Matrices
{
	mat4 data[MAX_OBJECTS];
} matrices;

layout(push_constant) uniform Constants
{
	vec3 offset;
	int matrixId;
	int materialId;
} constants;

layout(location = 0) out vec3 fragPosition;
layout(location = 2) out vec3 fragTexCoord;

void main()
{
	mat4 view = mat4(mat3(vp.view)); // TODO: Move this to CPU

	fragPosition = vertPosition;
	fragTexCoord = vertPosition;
	vec4 pos = vp.projection * view * vec4(vertPosition, 1.0f);
    gl_Position = pos.xyww;
}
