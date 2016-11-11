/*
 *  Simple vertex shader for lab 4
 */

#version 330 core

uniform mat4 modelView;
uniform mat4 projection;
in vec3 vPosition;
in vec3 vNormal;
out vec3 normal;
out vec3 position;

void main() {
  gl_Position = projection * modelView * vec4(vPosition,1.0f);
  position = vPosition;
  normal = vNormal;
}
