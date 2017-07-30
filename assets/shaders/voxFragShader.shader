#version 400


#extension GL_ARB_conservative_depth : enable 

uniform sampler2D	textDiffuse;
uniform vec2		tables[11];
uniform float		time;

in vec3 			pos_color;
in vec2 			uv;
in vec3 			normal;
flat in int			material;
in vec3				camPos;

layout(location = 0) out vec4 FragColor;

vec3 compute_specular(vec3 col, vec3 viewDir, vec3 pos, vec3 norm, float specS, vec3  lightpos)
{
	int i = 0;

	vec3 ray;
	vec3 reflectray;
	vec3 outter = vec3(0, 0, 0);
	float phong;

		ray = normalize(lightpos - pos);
		reflectray = reflect(-ray, norm);
		phong = pow(max(dot(viewDir, reflectray), 0.0), specS);
		phong *= max(0, dot(ray, norm));
	outter += phong * col;
	outter.x = min(1, outter.x);
	outter.y = min(1, outter.y);
	outter.z = min(1, outter.z);
	return (outter);
}

void main(void)
{
	float a = clamp(dot(normalize(vec3(0, 1000000000000.0f, 0) - pos_color), normal), 0.4, 1.0);

	vec2 t = mod(uv , 1.0f);
	t.xy += tables[material].xy;
	
	FragColor = vec4(texture(textDiffuse, t * 0.0625).rgb, 1) * a;
}
