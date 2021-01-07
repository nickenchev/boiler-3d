#version 450

#define MAX_MATERIALS 256

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec4 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragNormal;

struct Material
{
	vec4 baseColorFactor;
	bool useBaseTexture;
};

layout(set = 0, binding = 2) uniform Materials
{
	Material data[MAX_MATERIALS];
} materials;

layout(set = 1, binding = 0) uniform sampler2D baseTexSampler;

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
	if (material.useBaseTexture) // add switch for this
	{
		outAlbedo = texture(baseTexSampler, fragTexCoord) * material.baseColorFactor;
	}
	else
	{
		outAlbedo = fragColor * material.baseColorFactor;
	}

	if (outAlbedo.a < 0.7)
		discard;
	outNormal = vec4(fragNormal, 1);
}

