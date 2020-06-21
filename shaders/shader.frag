#version 450

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec4 fragColour;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragNormal;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColour;

void main()
{
	vec3 lightPosition = vec3(10, 2, 10);
	vec3 lightColour = vec3(1, 1, 1);
	float ambientStrength = 0.01;
	vec3 ambient = lightColour * ambientStrength;

	// diffuse lighting
	vec3 norm = normalize(fragNormal);
	vec3 lightDirection = normalize(lightPosition - fragPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColour;
	
	outColour = vec4(ambient + diffuse, 1) * fragColour;
	//outColour = texture(texSampler, fragTexCoord) * fragColour;
}
