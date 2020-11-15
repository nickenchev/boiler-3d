#version 450

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec4 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragNormal;

layout(push_constant) uniform Constants
{
	vec3 cameraPosition;
} constants;

layout(binding = 1) uniform sampler2D baseTexSampler;

layout(binding = 2) uniform Material
{
	bool useBaseTexture;
} material;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outAlbedo;
layout(location = 2) out vec4 outNormal;

void main()
{
	outPosition = vec4(fragPosition, 1);
	if (material.useBaseTexture) // add switch for this
	{
		outAlbedo = texture(baseTexSampler, fragTexCoord);
	}
	else
	{
		outAlbedo = fragColor;
	}
	outNormal = vec4(fragNormal, 1);
}

