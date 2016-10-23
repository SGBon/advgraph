/*
 *  Terrain frag shader
 */

#version 330 core

uniform mat4 modelView;
uniform mat4 projection;
uniform vec3 light;
uniform vec3 eye;

uniform sampler1D tex;
uniform vec2 minmax;

in vec3 position;
in vec3 normal;
out vec4 fragcolor;

float compnorm(float val, float mini, float maxi){
	return (val - mini)/(maxi - mini);
}

void main() {
	vec4 white = vec4(1.0,1.0,1.0,1.0);
	vec4 base = texture(tex,compnorm(position.z,minmax.x,minmax.y));
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
