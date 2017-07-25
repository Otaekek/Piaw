#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in float in_Material;
layout(location = 4) in float in_mat1;
layout(location = 5) in float in_mat2;
layout(location = 6) in float in_mat3;
layout(location = 7) in float in_mat4;
layout(location = 8) in float in_mat5;
layout(location = 9) in float in_mat6;
layout(location = 10) in float in_mat7;
layout(location = 11) in float in_mat8;
layout(location = 12) in float in_mat9;
layout(location = 13) in float in_mat10;
layout(location = 14) in float in_mat11;
layout(location = 15) in float in_mat12;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 camPos;
out vec3 pos_color;
out vec2 uv;
out vec3 normal;
out float matlist[12];

void main(void)
{
	vec4 tmpcamPos;
	tmpcamPos.x = -V[0][3];
	tmpcamPos.y = -V[1][3];
	tmpcamPos.z = -V[2][3];
	tmpcamPos.w = 0.0f;
	tmpcamPos *= inverse(V);
	camPos = tmpcamPos.xyz;
	uv = in_uv;
	gl_Position  = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
	gl_Position *= M;
	normal = in_Normal;
	pos_color = gl_Position.xyz;
//	material = int(in_Material);
	gl_Position *= V;
	gl_Position *= P;
	matlist[0] = in_mat1 / 255.0f;
	matlist[1] = in_mat2 / 255.0f;
	matlist[2] = in_mat3 / 255.0f;
	matlist[3] = in_mat4 / 255.0f;
	matlist[4] = in_mat5 / 255.0f;
	matlist[5] = in_mat6 / 255.0f;
	matlist[6] = in_mat7 / 255.0f;
	matlist[7] = in_mat8 / 255.0f;
	matlist[8] = in_mat9 / 255.0f;
	matlist[9] = in_mat10 / 255.0f;
	matlist[10] = in_mat11 / 255.0f;
	matlist[11] = in_mat12 / 255.0f;
}
