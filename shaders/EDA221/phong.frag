#version 410

uniform vec3 light_position;
uniform vec3 camera_position;
uniform vec3 specular;
uniform float shininess;
uniform vec3 diffuse;
uniform vec3 ambient;

in VS_OUT {
	vec3 vertex;
	vec3 normal;
} fs_in;

out vec4 frag_color;

void main()
{
    
	vec3 camera = normalize(camera_position - fs_in.vertex);
	vec3 view = normalize(light_position - fs_in.vertex);

    //Specular Calculations
	float spec_no_shine = clamp(dot(reflect(-view, normalize(fs_in.normal)), camera), 0.0, 1.0);
	float spec_shine = pow(spec_no_shine, shininess);
    vec3 spec_vec = spec_shine * normalize(specular);

    //Diffused Calculations
	float diff = clamp(dot(normalize(fs_in.normal), view), 0.0, 1.0);
    vec3 diffuse_vec = diff*normalize(diffuse);
    
    vec3 amb_vec = normalize(ambient);
    
    vec3 fin = spec_vec + diff_vec + amb_vec;
    frag_color = vec4(fin, 1.0);

}