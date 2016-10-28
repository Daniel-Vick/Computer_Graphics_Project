#version 410

uniform vec3 camera_position;
uniform vec3 light_position;
uniform sampler2D Sampler;
uniform sampler2D diffuse_texture;
uniform mat4 normal_model_to_world;

in VS_OUT {
	vec3 vertex;
	vec3 normal;
	vec2 texcoords;
	vec3 tangent;
	vec3 binormal;
} fs_in;

out vec4 frag_color;

void main()
{
	mat3 TBN = mat3(fs_in.tangent, fs_in.binormal, fs_in.normal);
	vec4 colors = texture(Sampler, fs_in.texcoords);
	vec3 newVec = colors.xyz*2 - 1;
	vec3 newNormal = (normal_model_to_world * vec4(TBN * newVec, 0)).xyz;


	vec3 camera = normalize(camera_position - fs_in.vertex);
	vec3 view = normalize(light_position - fs_in.vertex);

	float yee = clamp(dot(reflect(view, normalize(newNormal)), camera), 0.0, 1.0);
	float yaa = pow(yee, 100);


	float diff = clamp(dot(normalize(newNormal), view), 0.0, 1.0);




	
	vec4 tex = texture(diffuse_texture, fs_in.texcoords) * clamp(dot(normalize(newNormal), view), 0.0, 1.0);

	frag_color = vec4(yaa + tex.x + 0.2, yaa + tex.y + 0.2, + yaa + tex.z + 0.2, 1.0);
	//frag_color = vec4(yaa, yaa, yaa, 1.0);

}
