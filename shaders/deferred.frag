#version 450

layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput inPositions;
layout (input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput inAlbedos;
layout (input_attachment_index = 2, set = 0, binding = 2) uniform subpassInput inNormals;

layout(location = 0) out vec4 outColor;

void main()
{
	vec3 lightPosition = vec3(0, 10, 0);
	vec3 lightColour = vec3(1, 1, 1);
	float ambientStrength = 0.02;
	vec3 ambient = vec3(0.6, 0.6, 0.6) * ambientStrength;

	// diffuse lighting
	vec3 norm = normalize(vec3(subpassLoad(inNormals)));
	vec3 lightDirection = normalize(lightPosition - vec3(subpassLoad(inPositions)));
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColour;
	
	vec4 albedo = subpassLoad(inAlbedos).rgba;
	vec4 finalColor = vec4(ambient + diffuse, 1) * albedo;

	outColor = finalColor;
}

