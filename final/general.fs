/*
 *  Simple fragment sharder for Lab 2
 */

#version 330 core

in vec4 position;
in vec3 normal;
uniform vec4 base;
uniform vec3 Eye;
uniform vec3 light;
out vec4 fragcolor;

void main() {
	const vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	float diffuse;
	vec3 L = normalize(light - position.xyz);
	vec3 N = normalize(normal);
	vec3 H = normalize(L + (Eye - position.xyz));
	float specular;

	diffuse = dot(N,L);
	if(diffuse < 0.0) {
		diffuse = 0.0;
		specular = 0.0;
	} else {
		specular = pow(max(0.0, dot(N,H)),1000.0);
	}

	fragcolor = min(0.3*base + 0.7*diffuse*base + 0.7*white*specular, vec4(1.0));
	fragcolor.a = base.a;
}
