
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
    scene1.deltaCamPosition.z = 10.0;
  if (key == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
    Object obj = scene1.objects[0];
    obj.currentPos = glm::vec3(rand()/(float)RAND_MAX - 0.5,rand()/(float)RAND_MAX - 0.5,rand()/(float)RAND_MAX - 0.5)*10.0f;
    scene1.objects.push_back(obj);
  }
  
  if (key == GLFW_MOUSE_BUTTON_5 && action == GLFW_PRESS) {
    for (int i=0; i<scene1.objects.size(); i++) {
      scene1.objects[i].scale -= glm::vec3(0.1f, 0.1f, 0.1f);
    }
  }
  
  if (key == GLFW_MOUSE_BUTTON_6 && action == GLFW_PRESS) {
    for (int i=0; i<scene1.objects.size(); i++) {
      scene1.objects[i].scale += glm::vec3(0.1f, 0.1f, 0.1f);
    }
  }

  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  std:: cout << xpos << " " << ypos << std::endl;
}

void MySGLEngine::UserKeyHandling(int key, int action, int mods) {
  if (key == GLFW_KEY_W && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = 10.0;
  if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_S && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = -10.0;
  if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_A && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = 10.0;
  if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_D && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = -10.0;
  if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    scene1.deltaCamRotY = 2.0*M_PI/(float)8.0;
  if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
    scene1.deltaCamRotY = 0.0;
  
  if (key == GLFW_KEY_E && action == GLFW_PRESS)
    scene1.deltaCamRotY = -2.0*M_PI/(float)8.0;
  if (key == GLFW_KEY_E && action == GLFW_RELEASE)
    scene1.deltaCamRotY = 0.0;
  
  if (key == GLFW_KEY_X && action == GLFW_PRESS) {
    std::cout << "Pressed" << std::endl;
    for (int i=0; i<scene1.objects.size(); i++) {
      scene1.objects[i].scale += glm::vec3(0.05f, 0.05f, 0.05f);
    }
  }
  
  if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
    std::cout << "Pressed" << std::endl;
    for (int i=0; i<scene1.objects.size(); i++) {
      scene1.objects[i].scale -= glm::vec3(0.05f, 0.05f, 0.05f);
      if(scene1.objects[i].scale.x < 0) scene1.objects[i].scale.x=0;
      if(scene1.objects[i].scale.y < 0) scene1.objects[i].scale.y=0;
      if(scene1.objects[i].scale.z < 0) scene1.objects[i].scale.z=0;
    }
  }
}

void MySGLEngine::UserInit() {
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback_user);
  glfwSetMouseButtonCallback(window, mouse_callback_user);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
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


  obj1.scale = glm::vec3(1.0,1.0,1.0);

  obj1.currentPos = glm::vec3(0.0,0.0,0.0);
  scene1.objects.push_back(obj1);

  obj1.currentPos = glm::vec3(3.0,3.0,0.0);
  scene1.objects.push_back(obj1);
  
  obj1.currentPos = glm::vec3(3.0,-3.0,0.0);
  scene1.objects.push_back(obj1);
  
  obj1.currentPos = glm::vec3(-3.0,-3.0,0.0);
  scene1.objects.push_back(obj1);
  
  obj1.currentPos = glm::vec3(-3.0,3.0,0.0);
  scene1.objects.push_back(obj1);


  scene1.camPosition = glm::vec3(0.0f,0.0f,0.0f);
  scene1.camPositionOffset = glm::vec3(0.0,0.0,-10.0);

  return 0;
}

void ChangeSomeStuff() {
  scene1.objects[0].currentPos -= glm::vec3(-0.0000000001,-0.000001, 0.0);
}

void MySGLEngine::Render() {
  // RenderScene(scene1);

  //std::thread t1(&MySGLEngine::RenderScene, this, scene1);
  //t1.join();

  std::cout << "Starting rendering thread..." << std::endl;
  std::thread t1([&] { this->RenderScene(scene1); });

  /* while (!glfwWindowShouldClose(window)) {
    ChangeSomeStuff();
  } */
  
  std::cout << "Waiting for rendering thread to finish..." << std::endl;

  t1.join();
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
