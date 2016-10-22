#include <cstdio>
#include <fstream>
#include <chrono>
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

  //const unsigned int n = ret.init_res;
  const unsigned int hnum = ret.final_res * ret.final_res;

  ret.heights = new GLfloat[hnum];
  for(unsigned int i = 0;i<hnum;i++)
    ret.heights[i] = 0;

  for(unsigned int j = 0; j<ret.final_res;j+=ret.final_res-1){
    for(unsigned int i = 0;i<ret.final_res;i+=ret.final_res-1){
      infile >> ret.heights[one_d_index(i,j,ret.final_res)];
    }
  }
  infile.close();

  ter_generate(ret);
  return ret;
}

void ter_destroy(struct terrain& ter){
  delete ter.heights;
}

void ter_generate(struct terrain& ter){
  /* haha STL is so ugly */
  std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::normal_distribution<GLfloat> distribution(0.0,1.0);
  float h = 1.0f;

  for(unsigned int width = ter.final_res - 1;width > 1;width /=2,h/=2.0){
    const unsigned int half_width = width/2;

    /* square step, get average of points in square around midpoint */
    /* during iterations, x,y are top left corner of squares */
    for(unsigned int x = 0;x < ter.final_res - 1;x+=width){
        for(unsigned int y = 0;y<ter.final_res - 1;y+=width){
        float avg = ter.heights[one_d_index(x,y,ter.final_res)] + /* top left */
        ter.heights[one_d_index(x+width,y,ter.final_res)] + /* top right */
        ter.heights[one_d_index(x,y+width,ter.final_res)] + /* bottom left */
        ter.heights[one_d_index(x+width,y+width,ter.final_res)]; /* bottom right */
        avg = avg / 4.0f;
        avg = avg + (distribution(generator)*2*h) - h;
        ter.heights[one_d_index(x+half_width,y+half_width,ter.final_res)] = avg;
      }
    }

    /* diamond step, get average of points in diamond around midpoint */
    /* during iterations, x,y are center point of diamond */
    for(unsigned int x = 0;x < ter.final_res - 1; x+=half_width){
      for(unsigned int y = (x+half_width)%width;y<ter.final_res - 1;y+=width){
        float avg = ter.heights[one_d_index((x-half_width + ter.final_res-1)%(ter.final_res-1),y,ter.final_res)] +
        ter.heights[one_d_index((x+half_width)%(ter.final_res-1),y,ter.final_res)] +
        ter.heights[one_d_index(x,(y+half_width)%(ter.final_res-1),ter.final_res)] +
        ter.heights[one_d_index(x,(y-half_width+ter.final_res-1)%(ter.final_res-1),ter.final_res)];
        avg /= 4.0f;

        avg = avg + (distribution(generator)*2*h) - h;
        ter.heights[one_d_index(x,y,ter.final_res)] = avg;

        if(x == 0) ter.heights[one_d_index(ter.final_res-1,y,ter.final_res)] = avg;
        if(y == 0) ter.heights[one_d_index(x,ter.final_res-1,ter.final_res)] = avg;
      }
    }
  }
}
