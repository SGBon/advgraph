#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstdio>
#include "Shaders.h"
#include "terproc.h"

void displayFunc(){

  return;
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitContextVersion(4,3);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(320, 320);
  glutCreateWindow("Viewer");

  glewExperimental = GL_TRUE;
  GLenum error = glewInit();
  if (error != GLEW_OK) {
      printf("Error starting GLEW: %s\n", glewGetErrorString(error));
      exit(0);
  }

  glutDisplayFunc(displayFunc);

  /* read .ter file
   * calculate subdivisions
   * populate buffers */

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0);

  glutMainLoop();
  return 0;
}
