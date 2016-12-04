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
#include "helper.hpp"
#include "VAO.hpp"
#include "boid.hpp"

float eyex, eyey, eyez;

double theta, phi;
double r;

GLuint monkeyProgram;
GLuint groundProgram;

glm::mat4 projection;

#define NUM_TRIBE 4 /* number of tribe members per tribe */
#define GRID_LENGTH 34 /* number of grid sections */
#define GRID_EMPTY -1
#define GRID_OBSTACLE -2
#define GRID_OFFSET 18
#define BOID_Y_OFFSET 1.0f

std::vector<boid> boids;

struct VAO monkey;
struct VAO ground;

int grid[GRID_LENGTH][GRID_LENGTH]; /* value in grid is indice of boid inside the grid */

void printGrid();

void initBoids();

void updateBoids();

void init() {
    /* prepare the ground */
    VAO_init(&ground);
    ground.program = groundProgram;

    /* Load the ground obj into the VAO */
    VAO_loadObj(&ground,"ground.obj");

    /* prepare the monkey model */
    VAO_init(&monkey);
    monkey.program = monkeyProgram;

    /*  Load the monkey obj file into the VAO */
    VAO_loadObj(&monkey,"monkey.obj");

    /* initialize grid */
    for(unsigned int i = 0; i < GRID_LENGTH; i++){
      for(unsigned int j = 0; j < GRID_LENGTH;j++){
        grid[i][j] = GRID_EMPTY; /* nothing in grid */
      }
    }

    /* put the ground based obstacles into the grid */
    for(size_t i = 0; i < ground.num_vertices;i+=3){
      const float x = ground.vertices[i];
      const float y = ground.vertices[i+1];
      const float z = ground.vertices[i+2];
      if(y > 0){
        const unsigned int x_ind = getGridCell(x,GRID_OFFSET);
        const unsigned int z_ind = getGridCell(z,GRID_OFFSET);
        grid[x_ind-1][z_ind-1] = GRID_OBSTACLE;
      }
    }

    initBoids();
}

void printGrid(){
  for(unsigned int i = 0; i < GRID_LENGTH;i++){
    for(unsigned int j = 0;j < GRID_LENGTH;j++){
      if(grid[i][j] != GRID_EMPTY)
        printf("%2d ",grid[i][j]);
      else
        printf("   ");
    }
    printf("\n");
  }
}

void initBoids(){
  /* set up initial boid positions */
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-1,1);
  const float side = 16.0f;
  boid::setBounds(-side,-side,side,side);
  for(unsigned int i = 0; i < NUM_TRIBE;i++){
    unsigned int x_ind = 0;
    unsigned int z_ind = 0;
    float red_z;
    /* get random points for red boid, only assign if not shared
     * if there's already a boid in the grid slot, roll a new number */
    do{
      red_z = dis(gen) * side;
      /* get grid point */
      x_ind = getGridCell(side,GRID_OFFSET);
      z_ind = getGridCell(red_z,GRID_OFFSET);
    }while(grid[x_ind][z_ind] != GRID_EMPTY);

    /* set point in grid with this boid's index */
    grid[x_ind][z_ind] = i*2;

    boid redBoid(glm::vec3(side,BOID_Y_OFFSET,red_z),tribes::RED);
    redBoid.setAcceleration(glm::vec3(-1.0f,0.0f,0.0f));
    redBoid.setGoal(glm::vec3(-side+1,0.0f,0.0f));
    boids.push_back(redBoid);

    /* same thing but on blue tribe side */
    float blue_z;
    do{
      blue_z = dis(gen) * side;
      /* set grid point */
      x_ind = getGridCell(-side,GRID_OFFSET);
      z_ind = getGridCell(blue_z,GRID_OFFSET);
    }while(grid[x_ind][z_ind] != GRID_EMPTY);

    grid[x_ind][z_ind] = i*2 + 1;

    boid blueBoid(glm::vec3(-side,BOID_Y_OFFSET,blue_z),tribes::BLUE);
    blueBoid.setAcceleration(glm::vec3(1.0f,0.0f,0.0f));
    blueBoid.setGoal(glm::vec3(side+1,0.0f,0.0f));
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

      const glm::mat4 model = view*glm::translate(glm::mat4(1.0),boids[i].getPosition())
      * glm::rotate(glm::mat4(1.0f),
      (float)acos(glm::dot(boids[i].getDirection(),glm::vec3(0.0f,0.0f,1.0f))),
        glm::vec3(0.0f,1.0f,0.0f));

      viewLoc = glGetUniformLocation(mprog, "modelView");
      glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(model));

      projLoc = glGetUniformLocation(mprog, "projection");
      glUniformMatrix4fv(projLoc, 1, 0, glm::value_ptr(projection));

      eyeLoc = glGetUniformLocation(mprog,"Eye");
      glUniform3f(eyeLoc,eyex,eyey,eyez);

      colourLoc = glGetUniformLocation(mprog,"base");
      /* set colour based on tribe membership */
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
  /* search range among the grid cells */
  const unsigned int search_range = abs(floor(boid::FLOCK_RADIUS));

  for(unsigned int i = 0; i < boids.size();i++){
    /* original grid spots */
    const unsigned int ox = getGridCell(boids[i].getPosition().x,GRID_OFFSET);
    const unsigned int oz = getGridCell(boids[i].getPosition().z,GRID_OFFSET);

    /* get area around the boid */
    const unsigned int startx =
      (ox >= search_range) ? (ox - search_range) : 0;

    const unsigned int startz =
      (oz >= search_range) ? (oz - search_range) : 0;

    const unsigned int endx =
      ((GRID_LENGTH - ox) > search_range) ? (ox + search_range) : GRID_LENGTH;

    const unsigned int endz =
      ((GRID_LENGTH - oz) > search_range) ? (oz + search_range) : GRID_LENGTH;


    /* avoid obstacles */
    for(unsigned int j = startx; j < endx;j++){
      for(unsigned int k = startz;k<endz;k++){
        const unsigned int index = grid[j][k];
        if(index == GRID_OBSTACLE){
          const float rx = getReverseGrid(j,GRID_OFFSET);
          const float rz = getReverseGrid(k,GRID_OFFSET);
          const glm::vec3 obs_pos(rx,BOID_Y_OFFSET,rz);
          const float dist = glm::distance(obs_pos,boids[i].getPosition());
          /* determine if object is in path of boid */
          if(infront(boids[i],obs_pos,monkey.radius) || dist  < (monkey.radius+1.0f)){
            const glm::vec3 obs_dir = glm::normalize(obs_pos-boids[i].getPosition());
            const glm::vec3 dodge_dir(glm::normalize(boids[i].getDirection() - obs_dir));
            const float weight = 1.0f/glm::length(dodge_dir);

            /* smaller weight when distance is farther */
            boids[i].addAcceleration(dodge_dir*weight*0.5f);
            /* add a force that pushes away from the obstacle */
            boids[i].addAcceleration(-obs_dir*weight*0.05f);
          }
        }
      }
    }

    /* get average of nearby flock mates velocity
     * and compute a centroid
     */
    unsigned int count = 0;
    glm::vec3 average_velocity(0.0f,0.0f,0.0f);
    glm::vec3 centroid(0.0f,0.0f,0.0f);
    for(unsigned int j = startx ;j < endx;j++){
      for(unsigned int k = startz; k < endz;k++){
        const unsigned int index = grid[j][k];
        if(index != GRID_EMPTY && index != GRID_OBSTACLE){
          /* when the tribe is the same, get averages
          * for velocity and centroid matching
          */
          if(boids[index].getTribe() == boids[i].getTribe()){
            average_velocity += boids[index].getVelocity();
            centroid += boids[index].getPosition();
            count++;
          }

          /* compute boid-boid avoidance using dot product */
          const glm::vec3 currDirect = boids[i].getDirection();
          const glm::vec3 otherDirect = boids[i].getDirection();
          const float dist = glm::distance(boids[index].getPosition(),boids[i].getPosition());
          const float cosine = abs(glm::dot(currDirect,otherDirect));

          /* if cosine < 1 vectors intersect at some point
          * push intersection into future by increasing angle between
          * the boids. Also do the same if boids are too close
          */
          if((cosine < 1 || dist < monkey.radius) && index != i ){
            const float weight = 1.0f/(dist*dist);
            const glm::vec3 awayDirection(
              glm::normalize(
                boids[i].getPosition() - boids[index].getPosition()));
                boids[i].addAcceleration(awayDirection*weight*monkey.radius);
          }
        }
      }
    }

    average_velocity /= count;
    centroid /= count;
    const glm::vec3 centre_direction(glm::normalize(centroid - boids[i].getPosition()));
    const float currmag = glm::length(boids[i].getVelocity());
    const float avgmag = glm::length(average_velocity);

    /* keep boid moving in direction of goal */
    //if(glm::length(boids[i].getVelocity()) < 0.5f){
    if(glm::dot(boids[i].getDirection(),boids[i].goalDirection()) < 0.5f
  || glm::length(boids[i].getVelocity()) < 1.0f){
      boids[i].addAcceleration(boids[i].goalDirection()*0.8f);
    }

    /* keep boid at velocity of flock mates */
    if(currmag > avgmag){
      /* acceleration in reverse */
      boids[i].addAcceleration(-boids[i].getDirection());
    }else if (currmag < avgmag){
      /* acceleration forward */
      boids[i].addAcceleration(boids[i].getDirection());
    }

    /* keep boid within it's flock */
    boids[i].addAcceleration(centre_direction);

    boids[i].step(0.033f);

    /* new grid spots */
    const unsigned int nx = getGridCell(boids[i].getPosition().x,GRID_OFFSET);
    const unsigned int nz = getGridCell(boids[i].getPosition().z,GRID_OFFSET);
    grid[ox][oz] = GRID_EMPTY;
    if(grid[nx][nz] != GRID_OBSTACLE)
      grid[nx][nz] = i;
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
			r -= 0.5;
			break;
		case 'e':
			r += 0.5;
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
    fs = buildShader(GL_FRAGMENT_SHADER, "ground.fs");
    groundProgram = buildProgram(vs, fs, 0);
    dumpProgram(groundProgram, "ground shader");

    fs = buildShader(GL_FRAGMENT_SHADER,"monkey.fs");
    monkeyProgram = buildProgram(vs,fs,0);
    dumpProgram(monkeyProgram,"ground shader");

    theta = 1.5;
    phi = 1.5;
    r = 32.0;

    eyex = r*sin(theta)*cos(phi);
    eyey = r*sin(theta)*sin(phi);
    eyez = r*cos(theta);

    init();
    glutMainLoop();
}
