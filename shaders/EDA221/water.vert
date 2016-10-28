#version 410

layout (location = 0) in vec3 vertex;
layout (location = 2) in vec3 texcoords;

uniform mat4 vertex_model_to_world;
uniform mat4 normal_model_to_world;
uniform mat4 vertex_world_to_clip;
uniform float time;


out VS_OUT {
	vec3 vertex;
	vec3 normal;
    float dx;
    float dz;
    vec2 bumpCoord0;
    vec2 bumpCoord1;
    vec2 bumpCoord2;
    vec2 texcoords;
} vs_out;


void main()
{
    
    vec2 texScale = vec2(8,4);
    float bumpTime = mod(time, 100.0);
    vec2 bumpSpeed = vec2(-.05, 0);
    float A1 = 4.0;
    float f1 = 0.1;
    float p1 = 0.5;
    float k1 = 3.0;
    vec2 d1 = vec2(-1.0,0.0);
    
    float A2 = 2.5;
    float f2 = 0.1;
    float p2 = 0.3;
    float k2 = 2.0;
    vec2 d2 = vec2(-0.7,0.7);
    
    
    float newY1 = A1*pow((sin((d1.x*vertex.x + d1.y*vertex.z)*f1 + time*p1)*0.5 + 0.5),k1);
    float newY2 = A2*pow((sin((d2.x*vertex.x + d2.y*vertex.z)*f2 + time*p2)*0.5 + 0.5),k2);
    float newY = newY1+newY2;
    
    
    float dx1 = (0.5)*k1*f1*A1*pow((sin((d1.x*vertex.x+d1.y*vertex.z)*f1+time*p1)*0.5+0.5),(k1-1))*cos((d1.x*vertex.x+d1.y*vertex.z)*f1+time*p1)*d1.x;
    float dz1 = (0.5)*k1*f1*A1*pow((sin((d1.x*vertex.x+d1.y*vertex.z)*f1+time*p1)*0.5+0.5),(k1-1))*cos((d1.x*vertex.x+d1.y*vertex.z)*f1+time*p1)*d1.y;
    float dx2 = (0.5)*k2*f2*A2*pow((sin((d2.x*vertex.x+d2.y*vertex.z)*f2+time*p2)*0.5+0.5),(k2-1))*cos((d2.x*vertex.x+d2.y*vertex.z)*f2+time*p2)*d2.x;
    float dz2 = (0.5)*k2*f2*A2*pow((sin((d2.x*vertex.x+d2.y*vertex.z)*f2+time*p2)*0.5+0.5),(k2-1))*cos((d2.x*vertex.x+d2.y*vertex.z)*f2+time*p2)*d2.y;
    vs_out.dx = dx1 + dx2;
    vs_out.dz = dz1 + dz2;
    vs_out.texcoords = texcoords.xy;
    
    vs_out.bumpCoord0.xy = texcoords.xy*texScale + bumpTime*bumpSpeed;
    vs_out.bumpCoord1.xy = texcoords.xy*texScale*2 + bumpTime*bumpSpeed*4;
    vs_out.bumpCoord2.xy = texcoords.xy*texScale*4 + bumpTime*bumpSpeed*8;
    
    
    
    
  
	vs_out.vertex = vec3(vertex_model_to_world * vec4(vertex.x, newY, vertex.z, 1.0));
	vs_out.normal = vec3(normal_model_to_world * vec4(-(dx1+dx2),1,-(dz1 + dz2), 0.0));

	gl_Position = vertex_world_to_clip * vertex_model_to_world * vec4(vertex.x, newY, vertex.z, 1.0);
}



