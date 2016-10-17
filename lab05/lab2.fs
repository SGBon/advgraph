/*
 *  Simple fragment sharder for Lab 2
 */

#version 430 core

in vec3 normal;
in vec3 position;
in vec2 tc;
uniform sample2D tex;
uniform vec4 colour;
uniform vec3 Eye;
uniform vec3 light;
uniform vec4 material;
out vec4 fragcolor;

void main() {
  fragcolor = texture(tex,tc);
}