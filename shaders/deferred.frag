#version 450

layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput inPositions;
layout (input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput inAlbedos;
layout (input_attachment_index = 2, set = 0, binding = 2) uniform subpassInput inNormals;

layout(location = 0) out vec4 outColor;

void main()
{
	vec4 fragPosition = subpassLoad(inPositions);
	vec4 fragAlbedo = subpassLoad(inAlbedos);
	vec4 fragNormal = subpassLoad(inNormals);

	float lightRadius = 3;
	vec3 lightPosition = vec3(0, -5, 0);
	vec3 lightColour = vec3(1, 1, 1);
	float ambientStrength = 0.02;
	vec3 ambient = vec3(0.6, 0.6, 0.6) * ambientStrength;

	// diffuse lighting
	vec3 norm = normalize(vec3(fragNormal));
	vec3 lightDirection = normalize(lightPosition - vec3(fragPosition));
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColour;
	
	// calculate if fragment is within light
	float fragDist = distance(fragPosition.xyz, lightPosition.xyz);
	
	vec4 finalColor = (fragDist < lightRadius)
		? fragAlbedo * vec4(ambient + diffuse, 1)
		: fragAlbedo * vec4(ambient, 1);

	outColor = finalColor;
}

