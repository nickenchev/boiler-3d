#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1) in vec4 fragColour;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragNormal;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColour;

void main()
{
	vec3 lightColour = vec3(1, 1, 1);
	float ambientStrength = 0.1;
	vec3 ambient = lightColour * ambientStrength;
	
	outColour = vec4(ambient, 1) * fragColour;
	outColour = vec4(fragNormal, 1);
	//outColour = texture(texSampler, fragTexCoord) * fragColour;
}
