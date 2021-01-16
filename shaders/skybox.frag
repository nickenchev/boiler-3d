#version 450

#define MAX_MATERIALS 256

layout(location = 0) in vec3 fragPosition;
layout(location = 2) in vec3 fragTexCoord;

struct Material
{
	vec4 baseColorFactor;
	bool useBaseTexture;
};

layout(set = 0, binding = 2) uniform Materials
{
	Material data[MAX_MATERIALS];
} materials;

layout(set = 1, binding = 0) uniform samplerCube cubeSampler;

layout(push_constant) uniform Constants
{
	int matrixId;
	int materialId;
} constants;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outAlbedo;
layout(location = 2) out vec4 outNormal;

void main()
{
	Material material = materials.data[constants.materialId];
	outPosition = vec4(fragPosition, 1);
	outAlbedo = texture(cubeSampler, fragTexCoord);
}

