#version 430

// Input
layout(location = 0) in vec3 i_worldPos;
//layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_texCoord0;
layout(location = 3) in vec4 fragPosLightSpace;

// Output
out vec4 fragColor;

uniform sampler2D depthTexture;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture2D(depthTexture, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// check whether current frag pos is in shadow
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	return shadow;
}

void main()
{
	float rValue = texture2D(depthTexture, i_texCoord0.st).r;
	//rValue = (rValue + 1) * 0.5;
	rValue = (gl_FragCoord.z + 1) * 0.5;

	fragColor = vec4(rValue, rValue, 0.0, 1.0);
	
	//vec4 color = vec4(0.8, 0.8, 0.8, 1.0);
	//
	//// ambient
	//vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

	//// calculate shadow
	//float shadow = ShadowCalculation(fragPosLightSpace);
	//fragColor = (1.0 - shadow) * color + ambient;
}


//

//
//uniform mat4 projection;
//uniform mat4 view;
//uniform mat4 model;
//uniform mat4 lightSpaceMatrix;
//
//void main()
//{
//	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
//	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
//	vs_out.TexCoords = aTexCoords;
//	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
//	gl_Position = projection * view * model * vec4(aPos, 1.0);
//}