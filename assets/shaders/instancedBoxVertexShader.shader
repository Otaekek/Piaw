#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec3 in_Trans;
layout(location = 4) in vec3 in_Scale;
layout(location = 5) in vec3 in_Color;
//layout(location = 6) in vec3 in_Color;

uniform mat4 V;
uniform mat4 P;

out vec3 pos_color;
out vec2 uv;
out vec3 normal;
out vec3 col;

void main(void)
{
	uv = in_uv;
	gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
	gl_Position.xyz *= in_Scale;
	gl_Position.xyz += in_Trans;
	normal = (vec4(in_Normal.xyz, 0)).xyz;
	pos_color = gl_Position.xyz;
	col = in_Color;
	gl_Position *= V;
	gl_Position *= P;
	gl_Position.z = 0.1f * log(gl_Position.w) / log(10000000000.0f); 
	gl_Position.z *= gl_Position.w;
}
