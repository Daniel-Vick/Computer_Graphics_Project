#version 410

in VS_OUT {
	vec3 vertex;
	vec3 normal;
} fs_in;

uniform samplerCube Sampler;
out vec4 frag_color;

void main()
{

	frag_color = vec4(texture(Sampler, normalize(fs_in.normal)).xyz, 1.0);
}
