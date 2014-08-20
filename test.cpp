
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sglengine.h"

std::string vertshaderfilename, fragshaderfilename, objectfilename;

SGLEngine::Scene scene1;

static void key_callback_user(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback_user(GLFWwindow* window, int key, int action, int mods);

class MySGLEngine : public SGLEngine {
  public:
    int SetupScene();
    void UserInit();
    void UserKeyHandling(int key, int action, int mods);
    void UserMouseHandling(int key, int action, int mods);

  private:
    void Render();
};

MySGLEngine e;
void MySGLEngine::UserMouseHandling(int key, int action, int mods) {
  if (key == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = 0.5;
  if (key == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
}

void MySGLEngine::UserKeyHandling(int key, int action, int mods) {
  if (key == GLFW_KEY_W && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = 0.5;
  if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_S && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = -0.5;
  if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_A && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = 0.5;
  if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_D && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = -0.5;
  if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    scene1.deltaCamRotY = 0.01;
  if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
    scene1.deltaCamRotY = 0.0;
  
  if (key == GLFW_KEY_E && action == GLFW_PRESS)
    scene1.deltaCamRotY = -0.01;
  if (key == GLFW_KEY_E && action == GLFW_RELEASE)
    scene1.deltaCamRotY = 0.0;
}

void MySGLEngine::UserInit() {
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback_user);
  glfwSetMouseButtonCallback(window, mouse_callback_user);
}

int MySGLEngine::SetupScene() {
  SGLEngine::Object obj1;

  std::string filepath(objectfilename);
  if (ObjParser(filepath, obj1) != 0)
    std::cout << "Error loading object " << filepath << " !" << std::endl;

  glfwMakeContextCurrent(window);

  for(int i=0; i<obj1.vertices.size(); i++) {
    obj1.colors.push_back(rand()/(double)RAND_MAX);
  }
   

  obj1.isIndexed = true;
  SetupObject(obj1);
  obj1.shader = SGLEngine::LoadShaders( vertshaderfilename.c_str(), NULL, fragshaderfilename.c_str() );


  obj1.currentPos = glm::vec3(0.0,0.0,0.0);
  scene1.objects.push_back(obj1);

  obj1.currentPos = glm::vec3(3.0,3.0,3.0);
  scene1.objects.push_back(obj1);
  
  obj1.currentPos = glm::vec3(3.0,-3.0,3.0);
  scene1.objects.push_back(obj1);
  
  obj1.currentPos = glm::vec3(-3.0,-3.0,3.0);
  scene1.objects.push_back(obj1);
  
  obj1.currentPos = glm::vec3(-3.0,3.0,3.0);
  scene1.objects.push_back(obj1);


  scene1.camPosition = glm::vec3(10.0f,10.0f,-3.0f);
  scene1.camPositionOffset = glm::vec3(0.0,0.0,0.0);

  return 0;
}

void MySGLEngine::Render() {
  RenderScene(scene1);
}

static void key_callback_user(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  e.UserKeyHandling(key, action, mods);
}  

static void mouse_callback_user(GLFWwindow* window, int button, int action, int mods) {
  e.UserMouseHandling(button, action, mods);
}  

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cout << "./test.x object vertexshader fragmentshader" << std::endl;
    exit(1);
  }

  objectfilename = argv[1];
  vertshaderfilename = argv[2];
  fragshaderfilename = argv[3];

  e.Init();
  e.UserInit();

  std::cout << "Setting up scene..." << std::endl;
  e.SetupScene();

  std::cout << "Starting main loop..." << std::endl;
  e.Run();
}
