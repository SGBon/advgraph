#include "TerrainVAO.hpp"
#include "util.hpp"

TerrainVAO::TerrainVAO(struct terrain& ter){
  TerrainVAO();
  loadTer(ter);
}

TerrainVAO::TerrainVAO(){
  glGenVertexArrays(1,&this->id);
  glBindVertexArray(this->id);
  glGenBuffers(1,&this->vbuffer);
  glGenBuffers(1,&this->ibuffer);
}

void TerrainVAO::loadTer(struct terrain& ter){
  this->vertices = ter.heights;
  this->num_vertices = ter.final_res * ter.final_res;
  this->width = ter.final_res;
  linkVertices();
  populateBuffers();
}

void TerrainVAO::drawVAO(){
  glBindVertexArray(this->id);
  glDrawElements(GL_TRIANGLES,this->num_indices,GL_UNSIGNED_INT,NULL);
}

void TerrainVAO::linkVertices(){
  /* a quad on a grid is denoted by vertices at
   * (x,y),(x+1,y),(x,y+1),(x+1,y+1)
   * when 0 <= x,y <= width - 1 */
  unsigned int index = 0;
  for(unsigned int i = 0; i < this->width-1; i++){
    for(unsigned int j = 0; j < this->width-1; j++){
      /* first triangle of quad */
      this->indices[index++] = one_d_index(i,j,this->width);
      this->indices[index++] = one_d_index(i+1,j,this->width);
      this->indices[index++] = one_d_index(i,j+1,this->width);

      /* second triangle */
      this->indices[index++] = one_d_index(i+1,j,this->width);
      this->indices[index++] = one_d_index(i+1,j+1,this->width);
      this->indices[index++] = one_d_index(i,j+1,this->width);
    }
  }
}

void TerrainVAO::populateBuffers(){

}
