#version 400

layout(triangles,equal_spacing) in;
in vec3 tesPos[];
in vec3 tesNormal[];

out vec3 fsColor;

uniform mat4 uMvp;
uniform mat4 uView;

vec3 lerp3D(vec3 v0, vec3 v1, vec3 v2)
{
	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main(){
	vec3 pos = lerp3D(tesPos[0],tesPos[1],tesPos[2]);
	vec3 normal = lerp3D(tesNormal[0], tesNormal[1], tesNormal[2]);

	gl_Position = uMvp* vec4(pos, 1.0 );

	fsColor = doSpecularLight(normal, pos, uView);
}
