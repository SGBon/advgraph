/*
 *  Simple fragment sharder for Lab 2
 */

#version 330 core

in vec4 position;
in vec3 normal;
uniform vec3 Eye;
out vec4 fragcolor;

void main() {
	/* using the normal vector as the colour
	 * helps make the faces of the obstacles much
	 * more defined and looks cool
	 */
	vec4 colour = vec4(normal,1.0f);
	const vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	float diffuse;
	vec3 L = normalize(vec3(32.0, 0.5, 0.0) - position.xyz);
	vec3 N;
	vec3 H = normalize(L+(Eye - position.xyz));
	float specular;

	N = normalize(normal);
	diffuse = dot(N,L);
	if(diffuse < 0.0) {
		diffuse = 0.0;
		specular = 0.0;
	} else {
		specular = pow(max(0.0, dot(N,H)),1000.0);
	}

	fragcolor = min(0.3*colour + 0.7*diffuse*colour + 0.7*white*specular, vec4(1.0));
	fragcolor.a = colour.a;
}
