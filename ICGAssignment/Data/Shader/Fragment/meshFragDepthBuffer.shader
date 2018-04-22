#version 430

layout(location = 0) out float o_depth;

// Output
out vec4 fragColor;

void main()
{
	o_depth = gl_FragCoord.z;
	//o_depth = 0.3;
	//fragColor = vec4(0.0, 0.5, 0.5, 1.0);
}
