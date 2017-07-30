#version 400

#extension GL_ARB_conservative_depth : enable 

in vec3 			pos_color;
in vec2 			uv;
in vec3 			normal;
in vec3				col;

layout(location = 0) out vec4 FragColor;

void main(void)
{
	vec3 diffuse_color;
	//if (uv.x > 0.1 && uv.x < 0.9 && uv.y > 0.1 && uv.y < 0.9)
		diffuse_color = col;
	//else
	//	diffuse_color = vec3(1,1,1);
	vec3 frag_Normal = normalize(normal);

//	diffuse_color = texture(textDiffuse, uv).rgb;

	FragColor = vec4(diffuse_color / 2, 1);
}
