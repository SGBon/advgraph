#include <fstream>
#include <cstdio>
#include <random>
#include "terproc.hpp"
#include "util.hpp"

/* generates the terrain, performs subdivisions */
void ter_generate(struct terrain& ter);

struct terrain ter_read(std::string filename){
  /* read the .ter file */
  struct terrain ret;
  std::ifstream infile(filename);
  infile >> ret.world_size;
  infile >> ret.final_res;
  infile >> ret.init_res;

  const unsigned int n = ret.init_res;
  const unsigned int hnum = ret.final_res * ret.final_res;
  const unsigned int space = (ret.final_res/(n-1)) - 1; // space between initial grid points */
  printf("%u\n",space);
  ret.heights = new GLfloat[hnum];
  for(unsigned int i = 0;i<hnum;i++)
    ret.heights[i] = 0;

  for(unsigned int j = 0; j<ret.final_res;j+=ret.final_res-1){
    for(unsigned int i = 0;i<ret.final_res;i+=ret.final_res-1){
      printf("%d %d\n",i,j);
      infile >> ret.heights[one_d_index(i,j,ret.final_res)];
    }
  }
  infile.close();

  for(unsigned int j = 0;j<ret.final_res;j++){
    for(unsigned int i = 0;i<ret.final_res;i++){
      printf("%.2f ",ret.heights[one_d_index(i,j,ret.final_res)]);
    }
    printf("\n");
  }

  ter_generate(ret);
  return ret;
}

void ter_destroy(struct terrain& ter){
  delete ter.heights;
}

void ter_generate(struct terrain& ter){
  std::default_random_engine generator;
  std::normal_distribution<GLfloat> distribution(0.0,1.0);

  /* width of squares */
  unsigned int width = ter.final_res - 1;
}
