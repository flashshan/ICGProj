#version 430

// Input
layout(location = 0) in vec3 i_worldPos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_texCoord0;


// Output
out vec4 fragColor;

//uniform vec3 eyePosition;
uniform vec3 lightDirection;

uniform sampler2D diffuseTexture;
//uniform sampler2D specularTexture;

void main()
{
	vec4 _diffuseColor = texture2D(diffuseTexture, i_texCoord0.st);
	//vec4 _specularColor = texture2D(specularTexture, i_texCoord0.st);

	vec3 _ambientColor = vec3(0.1, 0.1, 0.1);

	//float specularPower = 5;

	vec3 lightDir = normalize(-lightDirection);
	vec3 normal = normalize(i_normal);
	float diffuseIntense = clamp(dot(lightDir, normal), 0.0, 1.0);
	//float diffuseIntense = 0.6;

	/*vec3 eyeDir = normalize(eyePosition - i_worldPos);
	vec3 halfLightEye = normalize((lightDir + eyeDir) * 0.5);
	float specularIntense = pow(clamp(dot(halfLightEye, normal), 0.0, 1.0), specularPower);*/

	//fragColor = vec4(normal.x, 0.0, normal.z, 0.0) + vec4(_ambientColor, 1.0);
	fragColor = _diffuseColor * diffuseIntense
		;

	//o_color = _diffuseColor * diffuseIntense + _specularColor * specularIntense + vec4(_ambientColor, 1.0);
	//o_color = _diffuseColor ;
	//o_color = _specularColor ;
}