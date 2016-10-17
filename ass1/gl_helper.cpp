#include "gl_helper.hpp"
#include <cstdio>
#include <vector>
#include "TerrainVAO.hpp"

static float eyex, eyey, eyez;

static double theta, phi;
static double r;

static GLuint program;

static glm::mat4 projection;

static std::vector<TerrainVAO> meshes;

static glm::vec3 camera;
static glm::vec3 direction;

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).

    if (h == 0)
        h = 1;

    float ratio = 1.0 * w / h;

    glViewport(0, 0, w, h);

    projection = glm::perspective(45.0f, ratio, 1.0f, 100.0f);

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
    glUseProgram(program);

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

    for(size_t i = 0;i<meshes.size();i++){
      meshes[i].drawVAO();
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
void gl_init(int argc, char** argv){
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


  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

void add_vao(TerrainVAO vao){
  meshes.push_back(vao);
}
