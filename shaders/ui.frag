#version 450

#define MAX_MATERIALS 1024

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec4 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragNormal;

layout(push_constant) uniform Constants
{
	vec4 offset;
	int matrixId;
	int materialId;
} constants;

struct Material
{
	vec4 baseColorFactor;
};

layout(set = 0, binding = 2) uniform Materials
{
	Material data[MAX_MATERIALS];
} materials;

layout(set = 1, binding = 0) uniform sampler2D baseTexSampler;

layout(location = 0) out vec4 outColor;

void main()
{
	Material material = materials.data[constants.materialId];
	vec4 tex = texture(baseTexSampler, fragTexCoord);
	outColor = tex * material.baseColorFactor * fragColor;
}

