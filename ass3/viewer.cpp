/************************************************************
*                   CSCI 4110 Lab 2
*
*  Basic OpenGL program that shows how to set up a
*  VAO and some basic shaders.  This program draws
*  a cube or sphere depending upon whether CUBE or
*  SPHERE is defined.
*
*/

#include <vector>
#include <random>
#include <cmath>
#include <cstdio>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include "Shaders.h"
#include "VAO.hpp"
#include "boid.hpp"

float eyex, eyey, eyez;

double theta, phi;
double r;

GLuint shaderProgram;

glm::mat4 projection;

/* number of tribe members per tribe */
#define NUM_TRIBE 5

std::vector<boid> boids;

struct VAO monkey;
struct VAO ground;

void updateBoids();

void init() {
    /* prepare the ground */
    VAO_init(&ground);
    ground.program = shaderProgram;

    /* Load the ground obj into the VAO */
    VAO_loadObj(&ground,"ground.obj");

    /* prepare the monkey model */
    VAO_init(&monkey);
    monkey.program = shaderProgram;

    /*  Load the monkey obj file into the VAO */
    VAO_loadObj(&monkey,"monkey.obj");

    /* set up initial boid positions */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1,1);
    const float base_height = 1.f;
    const float side = 16.0f;
    for(unsigned int i = 0; i < NUM_TRIBE;i++){
      const float red_z = dis(gen) * side;
      boid redBoid(glm::vec3(side,base_height,red_z),tribes::RED);
      redBoid.setAcceleration(glm::vec3(-1.0f,0.0f,0.0f));
      boids.push_back(redBoid);


      const float blue_z = dis(gen) * side;
      boid blueBoid(glm::vec3(-side,base_height,blue_z),tribes::BLUE);
      blueBoid.setAcceleration(glm::vec3(1.0f,0.0f,0.0f));
      boids.push_back(blueBoid);
    }
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
    GLuint colourLoc;

    const GLuint mprog = monkey.program;
    const glm::vec4 red(1.0f,0.0f,0.0f,1.0f);
    const glm::vec4 blue(0.0f,0.0f,1.0f,1.0f);
    const glm::vec4 brown(0.3f,0.3f,0.1f,1.0f);

    const glm::mat4 view = glm::lookAt(glm::vec3(eyex, eyey, eyez),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* draw the monkeys */
    for(size_t i = 0; i < boids.size();i++){
      glUseProgram(mprog);

      const glm::mat4 model = view*glm::translate(glm::mat4(1.0),boids[i].getPosition());

      viewLoc = glGetUniformLocation(mprog, "modelView");
      glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(model));

      projLoc = glGetUniformLocation(mprog, "projection");
      glUniformMatrix4fv(projLoc, 1, 0, glm::value_ptr(projection));

      eyeLoc = glGetUniformLocation(mprog,"Eye");
      glUniform3f(eyeLoc,eyex,eyey,eyez);

      colourLoc = glGetUniformLocation(mprog,"base");
      switch(boids[i].getTribe()){
        case tribes::RED:
          glUniform4fv(colourLoc,1,glm::value_ptr(red));
          break;
        case tribes::BLUE:
          glUniform4fv(colourLoc,1,glm::value_ptr(blue));
          break;
      }

      glBindVertexArray(monkey.id);
      glDrawElements(GL_TRIANGLES, monkey.num_indices, GL_UNSIGNED_INT, NULL);
    }

    /* draw the ground */
    const GLuint gprog = ground.program;
    glUseProgram(gprog);

    const glm::mat4 model = view;
    viewLoc = glGetUniformLocation(gprog, "modelView");
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(model));

    projLoc = glGetUniformLocation(gprog, "projection");
    glUniformMatrix4fv(projLoc, 1, 0, glm::value_ptr(projection));

    eyeLoc = glGetUniformLocation(gprog,"Eye");
    glUniform3f(eyeLoc,eyex,eyey,eyez);

    colourLoc = glGetUniformLocation(gprog, "base");
    glUniform4fv(colourLoc,1,glm::value_ptr(brown));

    glBindVertexArray(ground.id);
    glDrawElements(GL_TRIANGLES, ground.num_indices, GL_UNSIGNED_INT, NULL);

    glutSwapBuffers();
    updateBoids();
}

void idleFunc(void){
  updateBoids();
  glutPostRedisplay();
}

void updateBoids(){
  for(unsigned int i = 0; i < boids.size();i++){
    boids[i].step(0.1f);
  }
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
    glClearColor(1.0, 1.0, 1.0, 1.0);

    vs = buildShader(GL_VERTEX_SHADER, "general.vs");
    fs = buildShader(GL_FRAGMENT_SHADER, "general.fs");
    shaderProgram = buildProgram(vs, fs, 0);
    dumpProgram(shaderProgram, "general shader");

    init();

    theta = 1.5;
    phi = 1.5;
    r = 32.0;

    eyex = r*sin(theta)*cos(phi);
    eyey = r*sin(theta)*sin(phi);
    eyez = r*cos(theta);

    glutMainLoop();
}
