#version 430

// Input
layout(location = 0) in vec3 i_worldPos;
layout(location = 1) in vec3 i_normal;
//layout(location = 2) in vec2 i_texCoord0;


// Output
out vec4 fragColor;

uniform vec3 diffuseColor;
//uniform vec3 lightDirection;

void main()
{
	
	fragColor = vec4(diffuseColor, 1.0);
	//o_color = _diffuseColor ;
	//o_color = _specularColor ;
}
