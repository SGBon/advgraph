#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <vector>
#include "gl_helper.hpp"
#include "Camera.hpp"

#define ESC_KEY 27

static glm::mat4 projection;

static std::vector<TerrainVAO*> meshes;

static tdogl::Camera camera;

/* callback when exiting program */
void cleanup();

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
  int viewLoc;
  int projLoc;

  const glm::mat4 y2zup = glm::rotate(glm::mat4(1.0),0.0f,glm::vec3(1.0,0.0,0.0)); /* rotation matrix to make z up axis when y is */

  const glm::mat4 view = camera.view() * y2zup;
  const glm::mat4 projection = camera.projection();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(size_t i = 0;i<meshes.size();i++){
    TerrainVAO* currvao = meshes[i];
    GLuint program = currvao->getShader();
    glUseProgram(program);

    viewLoc = glGetUniformLocation(program, "modelView");
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    projLoc = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projLoc,1,0,glm::value_ptr(projection));

    currvao->printVerts();
    currvao->drawVAO();
  }
  glutSwapBuffers();
}

void mouseDragFunc(int x, int y){
  printf("%d %d\n",x,y);
  const float sensitivity = 0.1f;
  camera.offsetOrientation(y*sensitivity,x*sensitivity);
}

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
      camera.offsetPosition(-camera.right());
      break;
    case 'd':
      camera.offsetPosition(camera.right());
      break;
    case 'w':
      camera.offsetPosition(camera.forward());
      break;
    case 's':
      camera.offsetPosition(-camera.forward());
      break;
		case 'z':
      camera.offsetPosition(camera.up());
			break;
		case 'x':
      camera.offsetPosition(-camera.up());
			break;
    case ESC_KEY:
      exit(0);
      break;
    }
}

void idleFunc(void){
  glutWarpPointer(0,0);
  glutPostRedisplay();
}

void ter_gl_init(int argc, char** argv){
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
  glutIdleFunc(idleFunc);
  glutReshapeFunc(changeSize);
  glutKeyboardFunc(keyboardFunc);
  glutMotionFunc(mouseDragFunc);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.5, 0.5, 0.5, 1.0);

  camera.lookAt(glm::vec3(1.0,1.0,1.0));
  camera.setPosition(glm::vec3(1.0,4.0,1.0));

  glutSetCursor(GLUT_CURSOR_NONE);

  atexit(cleanup);
}

void add_vao(TerrainVAO* vao){
  meshes.push_back(vao);
}

void cleanup(){
  for(size_t i = 0; i < meshes.size();i++){
    delete meshes[i];
  }
  exit(0);
}
