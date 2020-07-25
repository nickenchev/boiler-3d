#version 450

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec4 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragNormal;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main()
{
	vec3 lightPosition = vec3(0, 15, 0);
	vec3 lightColour = vec3(1, 1, 1);
	float ambientStrength = 0.04;
	vec3 ambient = lightColour * ambientStrength;

	// diffuse lighting
	vec3 norm = normalize(fragNormal);
	vec3 lightDirection = normalize(lightPosition - fragPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColour;
	
	// change sample position to simulate pixelation
	/*float pixel_w = 13.3;
	float pixel_h = 10;
    float dx = pixel_w*(1./1024);
    float dy = pixel_h*(1./768);
    vec2 coord = vec2(dx*floor(fragTexCoord.x/dx),
					  dy*floor(fragTexCoord.y/dy));*/
	
	vec4 baseColor = texture(texSampler, fragTexCoord);

	//if (baseColor.a < 0.5) // pass this into the shader
		//discard;
	vec4 finalColor = vec4(ambient + diffuse, 1) * baseColor;

	outColor = finalColor;
}
