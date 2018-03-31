#version 430

// Input
layout(location = 0) in vec3 i_worldPos;
layout(location = 1) in vec3 i_texCoord0;

// Output
layout(location = 1) out vec4 o_color;

// Output
out vec4 fragColor;

uniform samplerCube cubeMap;


void main()
{
	fragColor = texture(cubeMap, i_texCoord0) + vec4(0.3, 0.3, 0.3, 1.0);
	o_color = fragColor;
}
