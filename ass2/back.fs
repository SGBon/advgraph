/*
 *  Simple fragment sharder for Lab 2
 */

#version 330 core

in vec3 normal;
in vec3 position;
uniform samplerCube tex;
out vec4 fragcolor;

void main() {
  vec3 tc;

  tc = reflect(-position,normal);
  fragcolor = texture(tex,tc);
}
