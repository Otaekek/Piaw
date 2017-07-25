#version 400

uniform vec3 diffuse;

uniform sampler2D textDiffuse;
in vec2 uv;

uniform float 	sunlight;
uniform int		has_diffuse;
uniform int		id;
uniform int		meshName;

layout(location = 0) out vec4 FragColor;

void main(void)
{
	if (has_diffuse == 0)
		FragColor = vec4(diffuse.xyz * sunlight, 1);
	else
		FragColor = vec4(texture(textDiffuse, uv).rgb * sunlight, 1);// * vec4(0.7, 0.2, 0.2, 1);
	//FragColor *= 2;
}
