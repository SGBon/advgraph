#include <cstdio>
#include "TerrainVAO.hpp"
#include "util.hpp"
#include <glm/glm.hpp>

TerrainVAO::TerrainVAO(GLuint sp, struct Texture1D& texture,struct terrain& ter)
:vertices(NULL),
normals(NULL),
indices(NULL),
texels(NULL),
ratio(ter.world_size / ter.final_res),
shader_program(sp),
min(ter.min),
max(ter.max)
{
  glGenVertexArrays(1,&this->id);
  loadTexture(texture);
  loadTer(ter);
  createNormals();
  populateBuffers();
}

TerrainVAO::~TerrainVAO(){
  printf("deconstructing %p\n",this);
  if(vertices)
    delete[] this->vertices;
  if(normals)
    delete[] this->normals;
  if(indices)
    delete[] this->indices;
  if(texels)
    delete[] this->texels;
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
}

void TerrainVAO::loadTexture(struct Texture1D& texture){
  this->num_texels = texture.size/COLOUR_WIDTH;
  this->texels = new GLfloat[texture.size];
  for(unsigned int i = 0;i < texture.size;i++){
    this->texels[i] = texture.colour[i];
  }
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

      /* second triangle */
      this->indices[index++] = one_d_index(i+1,j,this->width);
      this->indices[index++] = one_d_index(i+1,j+1,this->width);
      this->indices[index++] = one_d_index(i,j+1,this->width);
    }
  }
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
      this->vertices[offset + 3] = 1.0;
    }
  }
}

void TerrainVAO::createNormals(){
  glm::vec4* ntable = new glm::vec4[this->num_vertices];

  /* initialize ntable */
  for(unsigned int i = 0;i<this->num_vertices;i++){
    ntable[i].x = 0;
    ntable[i].y = 0;
    ntable[i].z = 0;
    ntable[i].w = 0;
  }

  /* loop over each triangle (each triangle is 3 successive indices) */
  for(unsigned int i = 0; i < num_indices; i+=3){
    /* get index to each vertex */
    const unsigned int vind1 = this->indices[i]*this->vertex_width;
    const unsigned int vind2 = this->indices[i+1]*this->vertex_width;
    const unsigned int vind3 = this->indices[i+2]*this->vertex_width;

    //printf("%d %d %d\n",vind1,vind2,vind3);

    const glm::vec3 vert1(this->vertices[vind1],this->vertices[vind1+1],this->vertices[vind1+2]);
    const glm::vec3 vert2(this->vertices[vind2],this->vertices[vind2+1],this->vertices[vind2+2]);
    const glm::vec3 vert3(this->vertices[vind3],this->vertices[vind3+1],this->vertices[vind3+2]);

    const glm::vec3 vec1(vert2 - vert1);
    const glm::vec3 vec2(vert3 - vert2);

    const glm::vec3 fnorm = glm::normalize(glm::cross(vec1,vec2));

    /* sum up face normals on vertex normals */
    const unsigned int indv1 = vind1/vertex_width;
    const unsigned int indv2 = vind2/vertex_width;
    const unsigned int indv3 = vind3/vertex_width;
    ntable[indv1].x += fnorm.x;
    ntable[indv1].y += fnorm.y;
    ntable[indv1].z += fnorm.z;
    ntable[indv1].w += 1.0;

    ntable[indv2].x += fnorm.x;
    ntable[indv2].y += fnorm.y;
    ntable[indv2].z += fnorm.z;
    ntable[indv2].w += 1.0;

    ntable[indv3].x += fnorm.x;
    ntable[indv3].y += fnorm.y;
    ntable[indv3].z += fnorm.z;
    ntable[indv3].w += 1.0;
  }

  /* create our vertex normals */
  this->normals = new GLfloat[this->num_vertices*normal_width];
  for(unsigned int i = 0; i < this->num_vertices;i++){
    const float den = ntable[i].w;
    const unsigned int nind = i*normal_width;
    glm::vec3 norm = glm::normalize(glm::vec3(ntable[i].x/den,ntable[i].y/den,ntable[i].z/den));

    normals[nind] = norm.x;
    normals[nind+1] = norm.y;
    normals[nind+2] = norm.z;
  }

  delete[] ntable;
}

void TerrainVAO::populateBuffers(){
  /* vertices are stored only as height values, need to supply x,y to OpenGL */
  const size_t vbsize = vertex_width*this->num_vertices * sizeof(GLfloat); /* size of vertex data in vbuffer */
  const size_t nbsize = normal_width*this->num_vertices * sizeof(GLfloat); /* ditto for normals */
  const size_t ibsize = this->num_indices*sizeof(GLuint); /* ditto indices buffer  */

  glBindVertexArray(this->id);

  GLfloat *inter = new GLfloat[vbsize + nbsize];
  size_t index = 0;
  for(size_t i = 0; i < this->num_vertices;i++){
    inter[index++] = this->vertices[vertex_width*i];
    inter[index++] = this->vertices[vertex_width*i+1];
    inter[index++] = this->vertices[vertex_width*i+2];
    inter[index++] = this->vertices[vertex_width*i+3];
    inter[index++] = this->normals[normal_width*i];
    inter[index++] = this->normals[normal_width*i+1];
    inter[index++] = this->normals[normal_width*i+2];
  }

  /* load vertex data into buffer */
  glGenBuffers(1,&this->vbuffer);
  glBindBuffer(GL_ARRAY_BUFFER,this->vbuffer);
  glBufferData(GL_ARRAY_BUFFER,(vbsize + nbsize),inter,GL_STATIC_DRAW);
  //glBufferSubData(GL_ARRAY_BUFFER,0,vbsize,this->vertices);
  //glBufferSubData(GL_ARRAY_BUFFER,vbsize,nbsize,this->normals);

  /* load indices into buffer */
  glBindVertexArray(this->id);
  glGenBuffers(1,&this->ibuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ibuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,ibsize,this->indices,GL_STATIC_DRAW);

  /* load texels into buffer */
  glGenTextures(1,&this->tbuffer);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_1D,this->tbuffer);
  glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA32F,this->num_texels,0,GL_RGBA,GL_FLOAT,this->texels);

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  /* link buffers to shader programs */
  glUseProgram(this->shader_program);
  GLint vPosition = glGetAttribLocation(this->shader_program,"vPosition");
  glVertexAttribPointer(vPosition,4,GL_FLOAT,GL_FALSE,(vertex_width+normal_width)*sizeof(GLfloat),0);
  glEnableVertexAttribArray(vPosition);

  GLint vNormal = glGetAttribLocation(this->shader_program,"vNormal");
  glVertexAttribPointer(vNormal,3,GL_FLOAT,GL_FALSE,(vertex_width+normal_width)*sizeof(GLfloat),(void*)(vertex_width*sizeof(GLfloat)));
  glEnableVertexAttribArray(vNormal);
}

void TerrainVAO::printVerts(){
  #ifdef DEBUG
  for(unsigned int i = 0;i<this->num_vertices*3;i+=3){
    printf("%.2f %.2f %.2f\n",vertices[i],vertices[i+1],vertices[i+2]);
  }
  printf("------------------\n");
  #endif
}

void TerrainVAO::printNormals(){
  #ifdef DEBUG
  printf("NORMALS:\n");
  for(unsigned int i = 0;i<this->num_vertices*3;i+=3){
    printf("%.2f %.2f %.2f\n",normals[i],normals[i+1],normals[i+2]);
  }
  printf("-------------------\n");
  #endif
}
