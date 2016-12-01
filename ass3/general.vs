/*
 *  Simple vertex shader for lab 4
 */

#version 330 core

uniform mat4 modelView;
uniform mat4 projection;
in vec3 vPosition;
in vec3 vNormal;
out vec4 position;
out vec3 normal;

void main() {

	gl_Position = projection * modelView * vec4(vPosition,1.0);
	normal = (modelView * vec4(vNormal,1.0)).xyz;
	position = gl_Position;

}
