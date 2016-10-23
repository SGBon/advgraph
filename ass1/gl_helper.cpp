#define _USE_MATH_INCLUDES
#include <cstdio>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gl_helper.hpp"
#include "Camera.hpp"

static glm::mat4 projection;

static std::vector<TerrainVAO*> meshes;

static tdogl::Camera camera;

/* default value */
static float movespeed = 1.0f;

const unsigned short WINDOW_WIDTH = 1024;
const unsigned short WINDOW_HEIGHT = 640;
const unsigned short WINDOW_CENTER_X = WINDOW_WIDTH/2;
const unsigned short WINDOW_CENTER_Y = WINDOW_HEIGHT/2;

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
  /* rotation matrix to make z up axis */
  const glm::mat4 y2zup = glm::rotate(glm::mat4(1.0),
    (float)-M_PI_2,
    glm::vec3(1.0,0.0,0.0));

  const glm::mat4 view = camera.view() * y2zup;
  const glm::mat4 projection = camera.projection();
  const glm::vec3 eye = camera.position();
  const glm::vec3 light = glm::vec3(0.0,0.0,1.0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(size_t i = 0;i<meshes.size();i++){
    TerrainVAO* currvao = meshes[i];
    GLuint program = currvao->getShader();
    glUseProgram(program);

    int viewLoc = glGetUniformLocation(program, "modelView");
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));

    int projLoc = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projLoc,1,0,glm::value_ptr(projection));

    int eyeLoc = glGetUniformLocation(program,"eye");
    glUniform3f(eyeLoc,eye.x,eye.y,eye.z);

    int lightLoc = glGetUniformLocation(program,"light");
    glUniform3f(lightLoc,light.x,light.y,light.z);

    int hrLoc = glGetUniformLocation(program,"heightratio");
    glUniform1f(hrLoc,1.0f);

    int mxLoc = glGetUniformLocation(program,"minmax");
    glUniform2f(mxLoc,currvao->min,currvao->max);

    currvao->drawVAO();
  }
  glutSwapBuffers();
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
}

void idleFunc(void){
  glutWarpPointer(WINDOW_CENTER_X,WINDOW_CENTER_Y);
  glutPostRedisplay();
}

void ter_gl_init(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitContextVersion(3,3);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Terrain Generator");

  glewExperimental = GL_TRUE;
  GLenum error = glewInit();
  if (error != GLEW_OK) {
      printf("Error starting GLEW: %s\n", glewGetErrorString(error));
      exit(0);
  }

  /* set glut callbacks */
  glutDisplayFunc(displayFunc);
  glutIdleFunc(idleFunc);
  glutReshapeFunc(changeSize);
  glutKeyboardFunc(keyboardFunc);
  glutMotionFunc(mouseDragFunc);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.5, 0.5, 0.5, 1.0);

  camera.lookAt(glm::vec3(1.0,1.0,1.0));
  camera.setPosition(glm::vec3(1.0,4.0,1.0));
  camera.setNearAndFarPlanes(0.01f,4096.0f);

  glutSetCursor(GLUT_CURSOR_NONE);

  //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  atexit(cleanup);
}


/* sets movespeed for camera
 * to move several grid units per movement
 */
void setMS(const float world_size,const float grid_res){
  movespeed = (world_size/grid_res) * 10.0f;
};

void add_vao(TerrainVAO* vao){
  meshes.push_back(vao);
}

void cleanup(){
  for(size_t i = 0; i < meshes.size();i++){
    delete meshes[i];
  }
  exit(0);
}
