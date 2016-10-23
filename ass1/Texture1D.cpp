#include <stdio.h>
#include "Texture1D.hpp"
#include "util.hpp"

void Texture1D_create(struct Texture1D& texture,unsigned int resolution,std::vector<glm::vec4> colours,int ranges[]){
  texture.size = resolution * COLOUR_WIDTH;
  texture.colour = new GLfloat[texture.size];
  size_t index = 0;

  /* assign colour values to ranges */
  for(size_t i = 0; i < colours.size();i++){
    for(int j = 0;j<ranges[i];j++){
      texture.colour[index++] = colours[i].r;
      texture.colour[index++] = colours[i].g;
      texture.colour[index++] = colours[i].b;
      texture.colour[index++] = colours[i].a;
    }
  }

  /* fill final ranges if there's still space left */
  const glm::vec4 lc = colours[colours.size()-1]; /* last colour */
  while(index < texture.size){
    texture.colour[index++] = lc.r;
    texture.colour[index++] = lc.g;
    texture.colour[index++] = lc.b;
    texture.colour[index++] = lc.a;
  }
}

void Texture1D_destroy(struct Texture1D& texture){
  delete[] texture.colour;
}
