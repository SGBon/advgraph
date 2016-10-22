/*
 *  Simple fragment sharder for Lab 2
 */

#version 330 core

in vec3 position;
in vec3 normal;
uniform vec4 colour;
uniform vec3 Eye;
uniform vec3 light;
uniform vec4 material;
out vec4 fragcolor;

void main() {
	vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	float diffuse;
	vec3 L = normalize(light - position);
	vec3 N;
	vec3 R = normalize(reflect(-L,normal));
	float specular;

	N = normalize(normal);
	diffuse = dot(N,L);
	if(diffuse < 0.0) {
		diffuse = 0.0;
		specular = 0.0;
	} else {
		specular = pow(max(0.0, dot(N,R)),material.w);
	}

	fragcolor = min(material.x*colour + material.y*diffuse*colour + material.z*white*specular, vec4(1.0));
	fragcolor.a = colour.a;

}
