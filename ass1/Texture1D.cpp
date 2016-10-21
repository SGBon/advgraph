#include <stdio.h>
#include "Texture1D.hpp"
#include "util.hpp"

GLuint genTexture1D(std::vector<glm::vec4> colours,int ranges[]){
  for(size_t i = 0; i < colours.size();i++){
    printf("%d: ",ranges[i]);
    printVec4(colours[i]);
  }
  return 0;
}
