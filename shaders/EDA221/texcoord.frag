#version 410

in VS_OUT {
	vec2 texcoord;
} fs_in;

uniform sampler2D Sampler;
out vec4 frag_color;

void main()
{
	frag_color = texture(Sampler, fs_in.texcoord);
}
