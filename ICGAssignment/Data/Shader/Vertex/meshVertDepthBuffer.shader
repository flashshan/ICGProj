#version 430

// Input
//======
layout(location = 0) in vec3 i_position;
//layout(location = 1) in vec3 i_normal;
//layout(location = 2) in vec2 i_texCoord0;

uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = lightSpaceMatrix * vec4(i_position, 1.0);
}
