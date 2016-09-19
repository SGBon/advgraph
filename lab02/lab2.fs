/*
 *  Simple fragment sharder for Lab 2
 */

#version 430 core

in vec4 position;
in vec3 normal;
out vec4 fragcolor;

void main() {
	vec4 base = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	float diffuse;
	vec3 L = vec3(1.0, 1.0, 0.0) - position.xyz;
	vec3 N;
	vec3 H = normalize(L + vec3(0.0, 0.0, 1.0));
	float specular;

	N = normalize(normal);
	diffuse = dot(N,L);
	if(diffuse < 0.0) {
		diffuse = 0.0;
		specular = 0.0;
	} else {
		specular = pow(max(0.0, dot(N,H)),100.0);
	}

	fragcolor = min(0.3*base + 0.7*diffuse*base + 0.7*white*specular, vec4(1.0));
	fragcolor.a = base.a;

}
