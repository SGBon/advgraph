/*
 *  Terrain vert shader
 */

#version 330 core

uniform mat4 modelView;
uniform mat4 projection;
in vec4 vPosition;
in vec3 vNormal;
out vec3 position;
out vec3 normal;

void main() {
	gl_Position = projection * modelView * vPosition;
	normal = vNormal;
	position = vPosition.xyz;
}
