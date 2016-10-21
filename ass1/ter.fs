/*
 *  Terrain frag shader
 */

#version 330 core

uniform mat4 modelView;
uniform mat4 projection;
uniform vec3 light;
uniform vec3 eye;
in vec3 position;
in vec3 normal;
out vec4 fragcolor;

void main() {
	vec4 white = vec4(1.0,1.0,1.0,1.0);
	vec4 base = vec4(1.0,0.0,0.0,0.0);
	float diffuse;
	vec3 L = normalize(light - position);
	vec3 N;
	vec3 H = normalize(L + (eye - position));
  float specular;

  N = normalize(normal);
  diffuse = dot(N,L);
  if(diffuse < 0.0){
    diffuse = 0.0;
    specular = 0.0;
  }else{
    specular = pow(max(0.0, dot(N,H)),100.0);
  }

	fragcolor = min(0.3*base + 0.7*diffuse*base + 0.7*white*specular, vec4(1.0));
	fragcolor.a = base.a;
}
