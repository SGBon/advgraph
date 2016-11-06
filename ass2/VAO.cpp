#include "VAO.h"
#include "tiny_obj_loader.h"
#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>

void VAO_genSphereTexels(struct VAO *vao);
void VAO_genCubeTexels(struct VAO *vao);

void VAO_init(struct VAO *vao){
  vao->t_state = VAO::undefined;
  vao->b_state = VAO::undefined;
}

void VAO_loadObj(struct VAO *vao, char *filename, unsigned char is_sphere){
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

  if(is_sphere){
    VAO_genSphereTexels(vao);
  }else{
    VAO_genCubeTexels(vao);
  }

  /* create and load vertex buffers */
  glGenBuffers(1,&vao->vbuffer);
  glBindBuffer(GL_ARRAY_BUFFER,vao->vbuffer);
  glBufferData(GL_ARRAY_BUFFER,(vao->num_vertices + vao->num_normals + vao->num_texels)*sizeof(GLfloat),NULL,GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER,0,vao->num_vertices*sizeof(GLfloat),vao->vertices);
  glBufferSubData(GL_ARRAY_BUFFER,vao->num_vertices*sizeof(GLfloat),vao->num_normals*sizeof(GLfloat),vao->normals);
  glBufferSubData(GL_ARRAY_BUFFER,(vao->num_vertices + vao->num_normals)*sizeof(GLfloat),vao->num_texels*sizeof(GLfloat),vao->texels);

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
  glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(vPosition);

  const GLint vNormal = glGetAttribLocation(vao->program,"vNormal");
  glVertexAttribPointer(vNormal,3,GL_FLOAT,GL_FALSE,0,(void*)(vao->num_vertices*sizeof(GLfloat)));
  glEnableVertexAttribArray(vNormal);

  const GLint vTex = glGetAttribLocation(vao->program,"vTex");
  glVertexAttribPointer(vTex,2,GL_FLOAT,GL_FALSE,0,(void*)((vao->num_vertices+vao->num_normals)*sizeof(GLfloat)));
  glEnableVertexAttribArray(vTex);

  vao->b_state = VAO::post_buffer;
}

void VAO_destroy(struct VAO *vao){
  if(vao->t_state == VAO::is_cubemap){
    delete[] vao->texture.c;
  }else if(vao->t_state == VAO::is_texture){
    delete[] vao->texture.t;
  }

  if(vao->b_state == VAO::post_buffer){
    delete[] vao->vertices;
    delete[] vao->normals;
    delete[] vao->texels;
    delete[] vao->indices;
  }
}

void VAO_loadCubeMap(struct VAO *vao, char *foldername){
  /* bind our vao to opengl */
  glBindVertexArray(vao->id);

  /* read cubemap from files */
  const Cube *texture = vao->texture.c = loadCube("vcc");

  glGenTextures(1,&vao->tbuffer);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,vao->tbuffer);

  /* put cubemaps into buffer */
  for(unsigned char i = 0;i<6;i++){
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGBA,texture->width,texture->height,0,GL_RGB,GL_UNSIGNED_BYTE,texture->data[i]);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

  vao->t_state = VAO::is_cubemap;
}

void VAO_genSphereTexels(struct VAO *vao){
  const unsigned int verts = vao->num_vertices / 3;
  vao->num_texels = 2*verts;
  vao->texels = new GLfloat[vao->num_texels];
  for(size_t i = 0;i < verts;i++){
    double x = vao->vertices[3*i];
    double y = vao->vertices[3*i+1];
    double z = vao->vertices[3*i+2];

    double th = atan2(x,z);
    double ph = atan2(y,sqrt(x*x+z*z));

    vao->texels[2*i] = fabs(th)/M_PI;
    vao->texels[2*i+1] = ph/M_PI;
  }
}

void VAO_genCubeTexels(struct VAO *vao){

}
