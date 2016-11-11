/*
 *  Simple fragment sharder for Lab 2
 */

#version 330 core

in vec3 normal;
in vec3 position;
uniform samplerCube tex;
uniform vec4 colour;
uniform vec3 Eye;
uniform vec3 light;
uniform vec4 material;
out vec4 fragcolor;

void main() {
  vec3 tc;

  tc = reflect(-position,normal);
  fragcolor = texture(tex,tc);
}
