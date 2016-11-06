/************************************************************
*                   CSCI 4110 Lab 2
*
*  Basic OpenGL program that shows how to set up a
*  VAO and some basic shaders.  This program draws
*  a cube or sphere depending upon whether CUBE or
*  SPHERE is defined.
*
*/

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include "Shaders.h"
#include "VAO.h"
#include <iostream>

float eyex, eyey, eyez;

double theta, phi;
double r;

GLuint program;

glm::mat4 projection;

#define NUM_OBJECTS 1

struct VAO objects[NUM_OBJECTS];

void init() {
    VAO_init(&objects[0]);
    objects[0].program = program;

    /*  Load the obj file into the VAO */
    VAO_loadObj(&objects[0],"sphere.obj",1);
    VAO_loadCubeMap(&objects[0],"vcc");
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).

    if (h == 0)
        h = 1;

    float ratio = 1.0 * w / h;

    glViewport(0, 0, w, h);

    projection = glm::perspective(45.0f, ratio, 1.0f, 100.0f);
    glutPostRedisplay();
}

void displayFunc(void) {
    glm::mat4 view;
    int viewLoc;
    int projLoc;
    int colourLoc;
    int eyeLoc;
    int lightLoc;
    int materialLoc;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0; i < NUM_OBJECTS;i++){
      glUseProgram(objects[i].program);

      view = glm::lookAt(glm::vec3(eyex, eyey, eyez),
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 1.0f));

      viewLoc = glGetUniformLocation(program, "modelView");
      glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
      projLoc = glGetUniformLocation(program, "projection");
      glUniformMatrix4fv(projLoc, 1, 0, glm::value_ptr(projection));

      colourLoc = glGetUniformLocation(program,"colour");
      glUniform4f(colourLoc,1.0,0.0,0.0,1.0);
      eyeLoc = glGetUniformLocation(program,"Eye");
      glUniform3f(eyeLoc,eyex,eyey,eyez);
      lightLoc = glGetUniformLocation(program,"light");
      glUniform3f(lightLoc,1.0,1.0,1.0);
      materialLoc = glGetUniformLocation(program,"material");
      glUniform4f(materialLoc,0.3,0.7,0.7,150.0);

      glBindVertexArray(objects[i].id);
      glDrawElements(GL_TRIANGLES, objects[i].num_indices, GL_UNSIGNED_INT, NULL);
    }
    glutSwapBuffers();
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
			r -= 0.1;
			break;
		case 'e':
			r += 0.1;
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
    glutInitWindowSize(320, 320);
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

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    vs = buildShader(GL_VERTEX_SHADER, "lab2.vs");
    fs = buildShader(GL_FRAGMENT_SHADER, "lab2.fs");
    program = buildProgram(vs, fs, 0);
    dumpProgram(program, "Lab 2 shader program");
    init();

    eyex = 0.0;
    eyez = 0.0;
    eyey = 10.0;

    theta = 1.5;
    phi = 1.5;
    r = 10.0;

    glutMainLoop();
}
