#include <stdio.h>
#include "Texture1D.hpp"
#include "util.hpp"

void Texture1D_create(struct Texture1D& texture,unsigned int resolution,std::vector<glm::vec4> colours,int ranges[]){
  texture.size = resolution * COLOUR_BYTE;
  texture.colour = new unsigned char[texture.size];
  size_t index = 0;

  /* assign colour values to ranges */
  for(size_t i = 0; i < colours.size();i++){
    for(int j = 0;j<ranges[i];j++){
      texture.colour[index++] = colours[i].r*COLOUR_RANGE;
      texture.colour[index++] = colours[i].g*COLOUR_RANGE;
      texture.colour[index++] = colours[i].b*COLOUR_RANGE;
      texture.colour[index++] = colours[i].a*COLOUR_RANGE;
    }
  }

  /* fill final ranges if there's still space left */
  const glm::vec4 lc = colours[colours.size()-1]; /* last colour */
  while(index < texture.size){
    texture.colour[index++] = lc.r*COLOUR_RANGE;
    texture.colour[index++] = lc.g*COLOUR_RANGE;
    texture.colour[index++] = lc.b*COLOUR_RANGE;
    texture.colour[index++] = lc.a*COLOUR_RANGE;
  }
}

void Texture1D_destroy(struct Texture1D& texture){
  delete[] texture.colour;
}
