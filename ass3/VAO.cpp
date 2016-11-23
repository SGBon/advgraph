#include "VAO.hpp"
#include "tiny_obj_loader.h"
#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>

void VAO_init(struct VAO *vao){
  vao->b_state = VAO::undefined;;
}

void VAO_loadObj(struct VAO *vao, char *filename){
  size_t i;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err = tinyobj::LoadObj(shapes, materials, filename, 0);

  if(err !=""){
    fprintf(stderr,"Error loading object file %s\n",filename);
    exit(-1);
  }

  glGenVertexArrays(1,&vao->id);
  glBindVertexArray(vao->id);

  /* get vertex coordinates, one dimensional array */
  vao->num_vertices = shapes[0].mesh.positions.size();
  vao->vertices = new GLfloat[vao->num_vertices];
  for(i = 0; i < vao->num_vertices;i++){
    vao->vertices[i] = shapes[0].mesh.positions[i];
  }

  /* get vertex normals */
  vao->num_normals = shapes[0].mesh.normals.size();
  vao->normals = new GLfloat[vao->num_normals];
  for(i = 0;i<vao->num_normals;i++){
    vao->normals[i] = shapes[0].mesh.normals[i];
  }

  /* create and load vertex buffers */
  glGenBuffers(1,&vao->vbuffer);
  glBindBuffer(GL_ARRAY_BUFFER,vao->vbuffer);
  glBufferData(GL_ARRAY_BUFFER,(vao->num_vertices + vao->num_normals)*sizeof(GLfloat),NULL,GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER,0,vao->num_vertices*sizeof(GLfloat),vao->vertices);
  glBufferSubData(GL_ARRAY_BUFFER,vao->num_vertices*sizeof(GLfloat),vao->num_normals*sizeof(GLfloat),vao->normals);

  /* get indices for triangles */
  vao->num_indices = shapes[0].mesh.indices.size();
  vao->indices = new GLuint[vao->num_indices];
  for(i = 0;i<vao->num_indices;i++){
    vao->indices[i] = shapes[0].mesh.indices[i];
  }

  /* load indices */
  glGenBuffers(1,&vao->ibuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vao->ibuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,vao->num_indices*sizeof(GLuint),vao->indices,GL_STATIC_DRAW);

  /* link the shader program with the vertex buffers */
  glUseProgram(vao->program);
  const GLint vPosition = glGetAttribLocation(vao->program,"vPosition");
  glVertexAttribPointer(vPosition,VERT_WIDTH,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(vPosition);

  const GLint vNormal = glGetAttribLocation(vao->program,"vNormal");
  glVertexAttribPointer(vNormal,NORM_WIDTH,GL_FLOAT,GL_FALSE,0,(void*)(vao->num_vertices*sizeof(GLfloat)));
  glEnableVertexAttribArray(vNormal);

  vao->b_state = VAO::post_buffer;
}

void VAO_destroy(struct VAO *vao){
  if(vao->b_state == VAO::post_buffer){
    delete[] vao->vertices;
    delete[] vao->normals;
    delete[] vao->indices;
  }
}
