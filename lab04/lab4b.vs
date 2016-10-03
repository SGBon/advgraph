/*
 *  Simple vertex shader for lab 4
 */

#version 430 core

uniform mat4 modelView;
uniform mat4 projection;
uniform vec4 colour;
uniform vec3 Eye;
uniform vec3 light;
uniform vec4 material;
in vec4 vPosition;
in vec3 vNormal;
out vec4 ocolour;

void main() {

	gl_Position = projection * modelView * vPosition;
	vec3 normal = vNormal;
	vec3 position = vPosition.xyz;

  vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	float diffuse;
	vec3 L = normalize(light);
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

	ocolour = min(material.x*colour + material.y*diffuse*colour + material.z*white*specular, vec4(1.0));
	ocolour.a = colour.a;

}
