#version 430

// Input
//======
layout(location = 0) in vec3 i_position;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec3 i_texCoord0;


// Output
layout(location = 0) out vec3 o_worldPos;
layout(location = 1) out vec3 o_normal;
layout(location = 2) out vec2 o_texCoord0;

uniform mat4 fullTransformMatrix;

void main()
{
	o_worldPos = i_position;
	o_normal = i_normal;
	o_texCoord0 = vec2(i_texCoord0);
	//gl_Position = vec4(i_position, 1.0);
	//gl_Position = fullTransformMatrix * vec4((i_position), 1.0);
	gl_Position = fullTransformMatrix * vec4(i_position.x, i_position.y, i_position.z, 1.0);

}
