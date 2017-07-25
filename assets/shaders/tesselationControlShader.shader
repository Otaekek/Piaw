#version 400

in vec3 tcsPos[];
in vec3 tcsNormal[];

layout(vertices=3) out; // (1)
out vec3 tesPos[];
out vec3 tesNormal[];

uniform float uTessLevel;

void main(){

	// (2)
	tesNormal[gl_InvocationID] = tcsNormal[gl_InvocationID];
	tesPos[gl_InvocationID] = tcsPos[gl_InvocationID];

	// (3)
	gl_TessLevelOuter[0] = uTessLevel;
	gl_TessLevelOuter[1] = uTessLevel;
	gl_TessLevelOuter[2] = uTessLevel;

	// (4)
	gl_TessLevelInner[0] = uTessLevel;
}
