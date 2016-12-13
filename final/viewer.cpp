/************************************************************
*                   CSCI 4110 Final project
*                         L-Systems
*
*/

#include <random>
#define _USE_MATH_DEFINES
#include <cmath>
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

    glViewport(0, 0, w, h);

    projection = glm::perspective(45.0f, ratio, 1.0f, 10000.0f);
    glutPostRedisplay();
}

void displayFunc(void) {
    GLuint viewLoc;
    GLuint projLoc;
    GLuint eyeLoc;
    GLuint baseLoc;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4 view = glm::lookAt(glm::vec3(eyex, eyey, eyez),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::vec4 black(0.0f,0.0f,0.0f,1.0f);

    /* draw the ground */
    const GLuint lprog = plant.program;
    glUseProgram(lprog);

    const glm::mat4 model = view;
    viewLoc = glGetUniformLocation(lprog, "modelView");
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(model));

    projLoc = glGetUniformLocation(lprog, "projection");
    glUniformMatrix4fv(projLoc, 1, 0, glm::value_ptr(projection));

    eyeLoc = glGetUniformLocation(lprog,"Eye");
    glUniform3f(eyeLoc,eyex,eyey,eyez);

    baseLoc = glGetUniformLocation(lprog,"base");
    glUniform4fv(baseLoc,1,glm::value_ptr(black));

    glBindVertexArray(plant.id);
    glDrawElements(GL_POINTS, plant.num_indices, GL_UNSIGNED_INT, NULL);

    glutSwapBuffers();
}

void idleFunc(void){
  glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y) {

    switch (key) {
    case 'a':
        phi -= 0.1;
        break;
    case 'd':
        phi += 0.1;
        break;
    case 'w':
        theta += 0.1;
        break;
    case 's':
        theta -= 0.1;
        break;
		case 'q':
			r -= 2.0;
			break;
		case 'e':
			r += 2.0;
			break;
    }

    eyex = r*sin(theta)*cos(phi);
    eyey = r*sin(theta)*sin(phi);
    eyez = r*cos(theta);

    glutPostRedisplay();

}

int main(int argc, char **argv) {
    int fs;
    int vs;

		glutInit(&argc, argv);
		glutInitContextVersion(3,3);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 640);
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
    glutIdleFunc(idleFunc);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 0.5, 1.0);

    vs = buildShader(GL_VERTEX_SHADER, "general.vs");
    fs = buildShader(GL_FRAGMENT_SHADER, "general.fs");
    shaderProgram = buildProgram(vs, fs, 0);
    dumpProgram(shaderProgram, "ground shader");

    theta = 1.5;
    phi = 1.5;
    r = 500.0;

    eyex = 0.0f;//r*sin(theta)*cos(phi);
    eyey = 0.0f;//r*sin(theta)*sin(phi);
    eyez = 500.0f;//r*cos(theta);

    init();
    glutMainLoop();
}
