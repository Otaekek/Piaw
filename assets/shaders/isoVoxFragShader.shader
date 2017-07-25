#version 400


#extension GL_ARB_conservative_depth : enable 

uniform sampler2D	textDiffuse;
uniform vec2		tables[11];
uniform float		time;

in vec3 			pos_color;
in vec2 			uv;
in vec3 			normal;
in float			matlist[12];
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
	float a = clamp(dot(normalize(vec3(0, 1000000000000.0f, 1000000000000.0f) - pos_color), normal), 0.4, 1.0);

	vec2 t = mod(uv * 2, 0.8f) + 0.1f;
	vec2 r;

//	FragColor = vec4(texture(textDiffuse, t * 0.0625).rgb, 1) * a;

	r = tables[0].xy;
	FragColor = vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[0], 1);
	r = tables[1].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[1], 1);
	r = tables[2].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[2], 1);
	r = tables[3].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[3], 1);
	r = tables[4].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[4], 1);
	r = tables[5].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[5], 1);
	r = tables[6].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[6], 1);
	r = tables[7].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[7], 1);
	r = tables[8].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[8], 1);
	r = tables[9].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[9], 1);
	r = tables[10].xy;
	FragColor += vec4(texture(textDiffuse, (t + r) * 0.0625).rgb * matlist[10], 1);
	FragColor *= a;
/*
	if (pos_color.y < 180 || camPos.y < 180)
	{
		FragColor = mix(vec4(0.3, 0.4, 0.7, 1), FragColor, 0.3);
	}
	a = (distance(camPos, pos_color) + 0)/ 5000;
	a = clamp(a, 0 , 1);
	FragColor = FragColor * (1.5f - a) + vec4(1,1,1,1) * a;
*/
}
