/*
 *  Terrain vert shader
 */

#version 330 core

uniform mat4 modelView;
uniform mat4 projection;
in vec4 vPosition;
//in vec3 vNormal;
out vec4 position;
out vec3 normal;

void main() {
	gl_Position = projection * modelView * vPosition;
	//normal = (modelView * vec4(vNormal,1.0)).xyz;
	position = gl_Position;
}
