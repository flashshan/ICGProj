#version 430

// Input
layout(location = 0) in vec3 i_worldPos;
layout(location = 1) in vec3 i_normal;

// Output
layout(location = 1) out vec4 o_color;

out vec4 fragColor;

uniform vec3 eyePosition;
uniform vec3 lightDirection;

uniform samplerCube cubeMap;

void main()
{
	vec3 eyeToPos = normalize(i_worldPos - eyePosition);
	vec3 reflected = reflect(eyeToPos, normalize(i_normal));

	vec4 _basicColor = texture(cubeMap, reflected);

	float specularPower = 10;

	vec3 lightDir = normalize(-lightDirection);
	vec3 normal = normalize(i_normal);
	float diffuseIntense = clamp(dot(lightDir, normal), 0.0, 1.0);

	vec3 eyeDir = normalize(eyePosition - i_worldPos);
	vec3 halfLightEye = normalize((lightDir + eyeDir) * 0.5);
	float specularIntense = pow(clamp(dot(halfLightEye, normal), 0.0, 1.0), specularPower);

	fragColor = _basicColor * (diffuseIntense + specularIntense + 0.5);
	o_color = fragColor;
	//fragColor = texture(cubeMap, reflected) + vec4(0.1, 0.1, 0.1, 1.0);
}
