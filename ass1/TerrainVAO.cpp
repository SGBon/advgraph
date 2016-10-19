#include <cstdio>
#include "TerrainVAO.hpp"
#include "util.hpp"

TerrainVAO::TerrainVAO(GLuint sp,struct terrain& ter)
:vertices(NULL),
indices(NULL),
ratio(ter.world_size / ter.final_res),
shader_program(sp)
{
  glGenVertexArrays(1,&this->id);
  loadTer(ter);
}

TerrainVAO::~TerrainVAO(){
  printf("deconstructing %p\n",this);
  if(vertices)
    delete[] this->vertices;
  if(indices)
    delete[] this->indices;
}

void TerrainVAO::setShader(GLuint sp){
  this->shader_program = sp;
}

GLuint TerrainVAO::getShader(){
  return this->shader_program;
}

void TerrainVAO::loadTer(struct terrain& ter){
  this->num_vertices = ter.final_res * ter.final_res;
  this->width = ter.final_res;
  this->num_indices = 6*(this->width - 1)*(this->width - 1);
  linkVertices();
  createVertices(ter.heights);
  populateBuffers();
}

void TerrainVAO::drawVAO(){
  glBindVertexArray(this->id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ibuffer);
  glDrawElements(GL_TRIANGLES,this->num_indices,GL_UNSIGNED_INT,NULL);
}

void TerrainVAO::linkVertices(){
  /* a quad on a grid is denoted by vertices at
   * (x,y),(x+1,y),(x,y+1),(x+1,y+1)
   * when 0 <= x,y <= width - 1 */
  this->indices = new GLuint[this->num_indices];
  unsigned int index = 0;
  for(unsigned int j = 0; j < this->width-1; j++){
    for(unsigned int i = 0; i < this->width-1; i++){
      /* first triangle of quad */
      this->indices[index++] = one_d_index(i,j,this->width);
      this->indices[index++] = one_d_index(i+1,j,this->width);
      this->indices[index++] = one_d_index(i,j+1,this->width);

      #ifdef DEBUG
      printf("%u ",this->indices[index-3]);
      printf("%u ",this->indices[index-2]);
      printf("%u\n",this->indices[index-1]);
      #endif

      /* second triangle */
      this->indices[index++] = one_d_index(i+1,j,this->width);
      this->indices[index++] = one_d_index(i+1,j+1,this->width);
      this->indices[index++] = one_d_index(i,j+1,this->width);

      #ifdef DEBUG
      printf("%u ",this->indices[index-3]);
      printf("%u ",this->indices[index-2]);
      printf("%u\n",this->indices[index-1]);
      #endif
    }
  }
  /* load indices into buffer */
  glBindVertexArray(this->id);
  glGenBuffers(1,&this->ibuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ibuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->num_indices*sizeof(GLuint),this->indices,GL_STATIC_DRAW);
}

void TerrainVAO::createVertices(GLfloat* heights){
  /* vertices are stored only as height values, need to supply x,y to OpenGL */
  const unsigned int vbsize = vertex_width*this->num_vertices; /* size of vertex data in vbuffer */
  this->vertices = new GLfloat[vbsize];
  for(unsigned int j = 0;j<this->width;j++){
    for(unsigned int i = 0;i<this->width;i++){
      const unsigned int offset = vertex_width*i + vertex_width*j*this->width;
      this->vertices[offset] = i * ratio;
      this->vertices[offset + 1] = j * ratio;
      this->vertices[offset + 2] = heights[one_d_index(i,j,this->width)];
    }
  }
}

void TerrainVAO::populateBuffers(){
  /* vertices are stored only as height values, need to supply x,y to OpenGL */
  const unsigned int vbsize = vertex_width*this->num_vertices; /* size of vertex data in vbuffer */

  /* load vertex data into buffer */
  glBindVertexArray(this->id);
  glGenBuffers(1,&this->vbuffer);
  glBindBuffer(GL_ARRAY_BUFFER,this->vbuffer);
  glBufferData(GL_ARRAY_BUFFER,(vbsize)*sizeof(GLfloat),NULL,GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER,0,vbsize*sizeof(GLfloat),this->vertices);

  /* link buffers to shader programs */
  glUseProgram(this->shader_program);
  GLint vPosition = glGetAttribLocation(this->shader_program,"vPosition");
  glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(vPosition);
}

void TerrainVAO::printVerts(){
  #ifdef DEBUG
  for(unsigned int i = 0;i<this->num_vertices*3;i+=3){
    printf("%.2f %.2f %.2f\n",vertices[i],vertices[i+1],vertices[i+2]);
  }
  printf("------------------\n");
  #endif
}
