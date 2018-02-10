#version 430

// Input
layout(location = 0) in vec3 i_worldPos;
layout(location = 1) in vec3 i_normal;

// Output
out vec4 o_color;

uniform vec3 eyePosition;
uniform vec3 lightDirection;

void main()
{
	vec3 _diffuseColor = vec3(0.5, 0.0, 0.0);
	vec3 _specularColor = vec3(0.0, 0.0, 0.9);
	vec3 _ambientColor = vec3(0.1, 0.1, 0.1);

	float specularPower = 5;

	vec3 lightDir = normalize(-lightDirection);
	vec3 normal = normalize(i_normal);
	float diffuseIntense = clamp(dot(lightDir, normal), 0.0, 1.0);

	vec3 eyeDir = normalize(eyePosition - i_worldPos);
	vec3 halfLightEye = normalize((lightDir + eyeDir) * 0.5);
	float specularIntense = pow(clamp(dot(halfLightEye, normal), 0.0, 1.0), specularPower);

	o_color = vec4(_diffuseColor * diffuseIntense + _specularColor * specularIntense + _ambientColor, 1.0);
}
