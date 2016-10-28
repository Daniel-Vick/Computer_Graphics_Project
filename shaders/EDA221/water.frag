#version 410

uniform vec3 light_position;
uniform vec3 camera_position;
uniform vec3 specular;
uniform float shininess;
uniform vec3 diffuse;
uniform vec3 ambient;
uniform samplerCube Sampler;
uniform sampler2D bumpSampler;

in VS_OUT {
	vec3 vertex;
	vec3 normal;
    float dx;
    float dz;
    vec2 bumpCoord0;
    vec2 bumpCoord1;
    vec2 bumpCoord2;
    vec2 texcoords;
} fs_in;

out vec4 frag_color;

void main()
{
    mat3 BTN = mat3(normalize(vec3(1,fs_in.dx, 0)), normalize(vec3(0,fs_in.dz,1)), normalize(vec3(-fs_in.dx,1,-fs_in.dz)));
    vec4 n0 = texture(bumpSampler, fs_in.bumpCoord0)*2 -1;
    vec4 n1 = texture(bumpSampler, fs_in.bumpCoord1)*2 -1;
    vec4 n2 = texture(bumpSampler, fs_in.bumpCoord2)*2 -1;
    vec4 nBump = normalize(n0+n1+n2);
    
    
    
    
    vec3 view = normalize(camera_position - fs_in.vertex);
    vec4 deep = vec4(0.0,0.0,0.1,1.0);
    vec4 shallow = vec4(0.0,0.5,0.5,1.0);
    vec3 n = normalize(BTN*nBump.xyz);
    vec3 n = normalize(fs_in.normal);
    float facing = 1 - max(dot(view,n), 0);
    vec3 R = normalize(reflect(-view,n));
    
    
    //Fresnel
    float R0 = 0.02037;
    float fastFresnel = R0 + (1-R0)*pow((1-dot(view,n)),5);
    
    //Refraction
    vec3 Ra = normalize(refract(view,n,1.33));
    

    
    frag_color = mix(deep, shallow, facing) + vec4(texture(Sampler, R).xyz, 1.0)*fastFresnel + vec4(texture(Sampler, Ra).xyz, 1.0)*(1-fastFresnel);

}