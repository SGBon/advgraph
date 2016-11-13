/*
 *  Simple fragment sharder for Lab 2
 */

#version 330 core

in vec3 normal;
in vec3 position;
uniform samplerCube tex;
uniform vec3 Eye;
uniform float radius;
uniform float angles;
uniform int samples;
out vec4 fragcolor;

void main() {
  vec3 U = cross(vec3(normal.x,normal.z,normal.y),normal);
  vec3 W = cross(U,normal);

  vec3 V;
  vec3 tc;
  fragcolor = vec4(0.0,0.0,0.0,0.0);

  int total = samples*samples;
  float interval = angles/samples;
  for(int i = 0;i < samples;i++){
    float theta = interval * i;
    for(int j = 0;j<samples;j++){
      float phi = interval * j;
      V = (radius*cos(theta)*U) + (radius*sin(phi)*W);
      tc = normalize(normal+V);
      fragcolor = fragcolor + texture(tex,tc);
    }
  }


  fragcolor = fragcolor/total;
}
