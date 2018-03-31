#version 430

// Input
//======
layout(location = 0) in vec3 i_position;

// Output
layout(location = 1) out vec3 o_texCoord0;

uniform mat4 fullTransformMatrix;

void main()
{
	o_texCoord0 = i_position;
	
	gl_Position = fullTransformMatrix * vec4(i_position.x, i_position.y, i_position.z, 1.0);
}
