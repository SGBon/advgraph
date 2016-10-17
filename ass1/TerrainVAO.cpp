#include <cstdio>
#include "TerrainVAO.hpp"
#include "util.hpp"

TerrainVAO::TerrainVAO(GLuint sp,struct terrain& ter)
:shader_program(sp){
  TerrainVAO();
  loadTer(ter);
}

TerrainVAO::TerrainVAO(){
  glGenVertexArrays(1,&this->id);
  glBindVertexArray(this->id);
  glGenBuffers(1,&this->vbuffer);
  glGenBuffers(1,&this->ibuffer);
}

void TerrainVAO::setShader(GLuint sp){
  this->shader_program = sp;
}

GLuint TerrainVAO::getShader(){
  return this->shader_program;
}

void TerrainVAO::loadTer(struct terrain& ter){
  this->vertices = ter.heights;
  this->num_vertices = ter.final_res * ter.final_res;
  this->width = ter.final_res;
  this->num_indices = 6*(this->width - 1)*(this->width - 1);
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
  this->indices = new GLuint[this->num_indices];
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
  /* vertices are stored only as height values, need to supply x,y to OpenGL */
  const unsigned int vw = 3; /* vertex width */
  const unsigned int vbsize = vw*this->num_vertices; /* size of vertex data in vbuffer */
  GLfloat* buff_verts = new GLfloat[vbsize];
  for(unsigned int j = 0;j<this->width;j++){
    for(unsigned int i = 0;i<this->width;i++){
      const unsigned int offset = vw*i + vw*j*this->width;
      buff_verts[offset] = i;
      buff_verts[offset + 1] = j;
      buff_verts[offset + 2] = this->vertices[one_d_index(i,j,this->width)];
    }
  }

  /* load vertex data into buffer */
  glBindBuffer(GL_ARRAY_BUFFER,this->vbuffer);
  glBufferData(GL_ARRAY_BUFFER,(vbsize)*sizeof(GLfloat),NULL,GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER,0,vbsize*sizeof(GLfloat),buff_verts);

  /* load indices into buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ibuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->num_indices*sizeof(GLuint),this->indices,GL_STATIC_DRAW);

  /* link buffers to shader programs */
  glUseProgram(this->shader_program);
  GLint vPosition = glGetAttribLocation(this->shader_program,"vPosition");
  glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(vPosition);

  delete[] buff_verts;
}
