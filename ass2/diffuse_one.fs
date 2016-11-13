/*
 *  Simple fragment sharder for Lab 2
 */

#version 330 core

in vec3 normal;
in vec3 position;
uniform samplerCube tex;
uniform vec3 Eye;
out vec4 fragcolor;

void main() {
  vec3 tc = reflect(-position,normal);
  vec4 fleccomp = texture(tex,normal);

  fragcolor = fleccomp;
}
