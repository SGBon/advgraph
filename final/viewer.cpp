/************************************************************
*                   CSCI 4110 Final project
*                         L-Systems
*
*/

#include <cstdio>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shaders.h"
#include "VAO.hpp"
#include "lsystem.hpp"
#include "Camera.hpp"
#include "glm_helpers.hpp"

static tdogl::Camera camera;
/* default value */
static float movespeed = 1.0f;

const unsigned short WINDOW_WIDTH = 1024;
const unsigned short WINDOW_HEIGHT = 640;
const unsigned short WINDOW_CENTER_X = WINDOW_WIDTH/2;
const unsigned short WINDOW_CENTER_Y = WINDOW_HEIGHT/2;

float eyex, eyey, eyez;

double theta, phi;
double r;

GLuint shaderProgram;

glm::mat4 projection;

struct VAO plant;

void init() {
  lsystem lsys;
  lsys.readlsystem("plant.lsys");

  plant.program = shaderProgram;
  VAO_init(&plant);
  VAO_loadlsystem(&plant,lsys);
}

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).

    if (h == 0)
        h = 1;

    float ratio = 1.0 * w / h;

    camera.setViewportAspectRatio(ratio);

    glViewport(0, 0, w, h);

    projection = camera.projection();
}

void displayFunc(void) {
    GLuint viewLoc;
    GLuint projLoc;
    GLuint eyeLoc;
    GLuint lightLoc;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4 view = camera.view();

    /* draw the plant */
    const GLuint pprog = plant.program;
    glUseProgram(pprog);

    glm::mat4 model = view;
    viewLoc = glGetUniformLocation(pprog, "modelView");
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(model));

    projLoc = glGetUniformLocation(pprog, "projection");
    glUniformMatrix4fv(projLoc, 1, 0, glm::value_ptr(projection));

    eyeLoc = glGetUniformLocation(pprog,"Eye");
    glUniform3fv(eyeLoc,1,glm::value_ptr(camera.position()));

    lightLoc = glGetUniformLocation(pprog,"light");
    glUniform3f(lightLoc,0.0,10.0,10.0);

    glBindVertexArray(plant.id);
    glDrawElements(GL_TRIANGLES, plant.num_indices, GL_UNSIGNED_INT, NULL);

    glutSwapBuffers();
}

void idleFunc(void){
  glutWarpPointer(WINDOW_CENTER_X,WINDOW_CENTER_Y);
  glutPostRedisplay();
}

void mouseDragFunc(int x, int y){
  const float sensitivity = 0.1f;
  camera.offsetOrientation((y - WINDOW_CENTER_Y)*sensitivity,(x - WINDOW_CENTER_X)*sensitivity);
}

void keyboardFunc(unsigned char key, int x, int y) {
  #define ESC_KEY 27
  switch (key) {
  case 'a':
    camera.offsetPosition(-camera.right()*movespeed);
    break;
  case 'd':
    camera.offsetPosition(camera.right()*movespeed);
    break;
  case 'w':
    camera.offsetPosition(camera.forward()*movespeed);
    break;
  case 's':
    camera.offsetPosition(-camera.forward()*movespeed);
    break;
	case 'z':
    camera.offsetPosition(camera.up()*movespeed);
		break;
	case 'x':
    camera.offsetPosition(-camera.up()*movespeed);
		break;
  case ESC_KEY:
    exit(0);
    break;
  }
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  int fs;
  int vs;

	glutInit(&argc, argv);
	glutInitContextVersion(3,3);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Viewer");

	glewExperimental = GL_TRUE;
  GLenum error = glewInit();
  if (error != GLEW_OK) {
      printf("Error starting GLEW: %s\n", glewGetErrorString(error));
      exit(0);
  }

  glutDisplayFunc(displayFunc);
  glutReshapeFunc(changeSize);
  glutKeyboardFunc(keyboardFunc);
  glutMotionFunc(mouseDragFunc);
  glutIdleFunc(idleFunc);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.5, 0.5, 0.5, 1.0);

  vs = buildShader(GL_VERTEX_SHADER, "general.vs");
  fs = buildShader(GL_FRAGMENT_SHADER, "general.fs");
  shaderProgram = buildProgram(vs, fs, 0);
  dumpProgram(shaderProgram, "ground shader");

  camera.setNearAndFarPlanes(1.0f,1000.0f);
  camera.setPosition(glm::vec3(0.0f,30.0f,100.0f));

  init();
  glutMainLoop();
}
