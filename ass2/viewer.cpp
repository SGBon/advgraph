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

float eyex, eyey, eyez;

double theta, phi;
double r;

GLuint frontShader;
GLuint backShader;

glm::mat4 projection;

#define NUM_OBJECTS 2

struct VAO objects[NUM_OBJECTS];

void init() {
    /* prepare the primary sphere */
    VAO_init(&objects[0]);
    objects[0].program = frontShader;

    /*  Load the obj file into the VAO */
    VAO_loadObj(&objects[0],"sphere.obj");
    VAO_loadCubeMap(&objects[0],"vcc",0);

    /* prepare the background sphere */
    VAO_init(&objects[1]);
    objects[1].program = backShader;

    /* load obj into VAO */
    VAO_loadObj(&objects[1],"sphere.obj");
    VAO_loadCubeMap(&objects[1],"vcc",1);
    VAO_setTransform(&objects[1],glm::scale(glm::mat4(1.0f),glm::vec3(10.0f,10.0f,10.0f)));
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
    GLuint viewLoc;
    GLuint projLoc;
    GLuint eyeLoc;
    GLuint texLoc;
    GLuint radiusLoc;
    GLuint thetaLoc;
    GLuint samplesLoc;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(size_t i = 0; i < NUM_OBJECTS;i++){
      const GLuint program = objects[i].program;
      glUseProgram(program);

      const glm::mat4 view = glm::lookAt(glm::vec3(eyex, eyey, eyez),
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 1.0f));
      const glm::mat4 model = view*objects[i].transform;

      viewLoc = glGetUniformLocation(program, "modelView");
      glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(model));

      projLoc = glGetUniformLocation(program, "projection");
      glUniformMatrix4fv(projLoc, 1, 0, glm::value_ptr(projection));

      eyeLoc = glGetUniformLocation(program,"Eye");
      glUniform3f(eyeLoc,eyex,eyey,eyez);

      texLoc = glGetUniformLocation(program,"tex");
      glUniform1i(texLoc,i);

      radiusLoc = glGetUniformLocation(program,"radius");
      glUniform1f(radiusLoc,0.1f);

      thetaLoc = glGetUniformLocation(program,"angles");
      glUniform1f(thetaLoc,M_PI*2);

      samplesLoc = glGetUniformLocation(program,"samples");
      glUniform1i(samplesLoc,15);

      glActiveTexture(GL_TEXTURE0+i);
      glBindTexture(GL_TEXTURE_CUBE_MAP,objects[i].tbuffer);

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

    vs = buildShader(GL_VERTEX_SHADER, "general.vs");
    fs = buildShader(GL_FRAGMENT_SHADER, "diffuse_two.fs");
    frontShader = buildProgram(vs, fs, 0);
    dumpProgram(frontShader, "front sphere shader");

    fs = buildShader(GL_FRAGMENT_SHADER,"back.fs");
    backShader = buildProgram(vs,fs,0);
    dumpProgram(backShader,"Background sphere shader");
    init();

    eyex = 0.0;
    eyez = 0.0;
    eyey = 3.0;

    theta = 1.5;
    phi = 1.5;
    r = 3.0;

    glutMainLoop();
}
