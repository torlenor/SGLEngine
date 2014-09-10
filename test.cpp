
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
#include <glm/gtx/rotate_vector.hpp>

#include "sglengine.h"

std::string scenefilename;

SGLEngine::Scene scene1;

static void key_callback_user(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback_user(GLFWwindow* window, int key, int action, int mods);
static void mouse_move_callback_user(GLFWwindow* window, double xpos, double ypos);

float xOrigin = -1;
float yOrigin = -1;

class MySGLEngine : public SGLEngine {
  public:
    int SetupScene();
    void UserInit();
    void UserKeyHandling(int key, int action, int mods);
    void UserMouseHandling(int key, int action, int mods);
    void UserMouseMoveHandling(double xpos, double ypos);

  private:
    void Render();
};

MySGLEngine e;

void GetInitialVelocity(glm::vec3 &vel) {
  // initially we look in direction (0,0,1)
  // we also per default shoot in direction (0,0,1)
  vel = glm::vec3(0.0,0.0,60.0f);
  // we now rotate the vector
  float curCamRotY = scene1.camRotY;
  // vel = glm::rotateY(vel, curCamRotY/((float)(360.0*M_PI*2.0)));
  float angleY=curCamRotY;

  vel = glm::vec3(cos(angleY)*vel.x + sin(angleY)*vel.z, vel.y, -sin(angleY)*vel.x + cos(angleY)*vel.z);
  
  std::cout << vel.x << " " << vel.y << " " << vel.z << std::endl;
  
}

void MySGLEngine::UserMouseHandling(int key, int action, int mods) {
  if (key == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
    Object obj = scene1.objects.back();
    // obj.currentPos = glm::vec3(22.0f*rand()/(float)RAND_MAX, 22.0f*rand()/(float)RAND_MAX, -12.0f);
    obj.currentPos = scene1.camPosition;
    // obj.currentVel = glm::vec3(18.0*rand()/(float)RAND_MAX - 9.0, 4.0*rand()/(float)RAND_MAX,30.0);
    glm::vec3 vel;
    GetInitialVelocity(vel);
    obj.currentVel = vel;
    obj.gravMass = 100;
    obj.physGrav = true;
    obj.doColl = true;
    scene1.objects.push_back(obj);
  }

  if (key == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      xOrigin = xpos;
      yOrigin = ypos;
  }
  if (key == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE) {
      xOrigin = -1;
      yOrigin = -1;
  }

  if (key == GLFW_MOUSE_BUTTON_5 && action == GLFW_PRESS) {
    for (unsigned int i=0; i<scene1.objects.size(); i++) {
      scene1.objects[i].scale -= glm::vec3(0.1f, 0.1f, 0.1f);
    }
  }
  
  if (key == GLFW_MOUSE_BUTTON_6 && action == GLFW_PRESS) {
    for (unsigned int i=0; i<scene1.objects.size(); i++) {
      scene1.objects[i].scale += glm::vec3(0.1f, 0.1f, 0.1f);
    }
  }

  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  std:: cout << xpos << " " << ypos << std::endl;
}

void MySGLEngine::UserMouseMoveHandling(double xpos, double ypos) {
  if (xOrigin >= 0 || yOrigin >= 0) {
    // scene1.deltaCamRotY = -(xpos - xOrigin) * 0.01f;
    scene1.camRotY -= (xpos - xOrigin) * 0.005f;
    xOrigin = xpos;
  }
}

void MySGLEngine::UserKeyHandling(int key, int action, int mods) {
  if (key == GLFW_KEY_W && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = 20.0;
  if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_S && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = -20.0;
  if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_A && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = 20.0;
  if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_D && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = -20.0;
  if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_X && action == GLFW_PRESS)
    scene1.deltaCamPosition.y = -20.0;
  if (key == GLFW_KEY_X && action == GLFW_RELEASE)
    scene1.deltaCamPosition.y = 0.0;
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    scene1.deltaCamPosition.y = 20.0;
  if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    scene1.deltaCamPosition.y = 0.0;
  
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    scene1.deltaCamRotY = 1.5*M_PI/(float)2.0;
  if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
    scene1.deltaCamRotY = 0.0;
  
  if (key == GLFW_KEY_E && action == GLFW_PRESS)
    scene1.deltaCamRotY = -1.5*M_PI/(float)2.0;
  if (key == GLFW_KEY_E && action == GLFW_RELEASE)
    scene1.deltaCamRotY = 0.0;
  
  if (key == GLFW_KEY_R && action == GLFW_PRESS)
    scene1.deltaCamRotZ = 1.5*M_PI/(float)2.0;
  if (key == GLFW_KEY_R && action == GLFW_RELEASE)
    scene1.deltaCamRotZ = 0.0;
  
  if (key == GLFW_KEY_F && action == GLFW_PRESS)
    scene1.deltaCamRotZ = -1.5*M_PI/(float)2.0;
  if (key == GLFW_KEY_F && action == GLFW_RELEASE)
    scene1.deltaCamRotZ = 0.0;
  
  if (key == 93 && action == GLFW_PRESS) {
    for (unsigned int i=0; i<scene1.objects.size(); i++) {
      scene1.objects[i].scale += glm::vec3(0.05f, 0.05f, 0.05f);
    }
  }
  
  if (key == 47 && action == GLFW_PRESS) {
    for (unsigned int i=0; i<scene1.objects.size(); i++) {
      scene1.objects[i].scale -= glm::vec3(0.05f, 0.05f, 0.05f);
      if(scene1.objects[i].scale.x < 0) scene1.objects[i].scale.x += 0.05;
      if(scene1.objects[i].scale.y < 0) scene1.objects[i].scale.y += 0.05;
      if(scene1.objects[i].scale.z < 0) scene1.objects[i].scale.z += 0.05;
    }
  }
  
  if (key == GLFW_KEY_O && action == GLFW_PRESS) {
    if ( LoadScene(scene1, scenefilename) != 0) 
      exit(1);
  }

  if (key == GLFW_KEY_G && action == GLFW_PRESS) {
    for (auto &obj : scene1.objects ) 
      obj.physGrav = true;
  }
  
}

void MySGLEngine::UserInit() {
  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback_user);
  glfwSetMouseButtonCallback(window, mouse_callback_user);

  glfwSetCursorPosCallback  (window, mouse_move_callback_user);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetWindowTitle(window, "My OpenGL Engine");
}

int MySGLEngine::SetupScene() {

  glfwMakeContextCurrent(window);

  if ( LoadScene(scene1, scenefilename) != 0) 
    return 1;

  // Set initial camera position
  scene1.camPosition.x = -50;
  scene1.camPosition.y = 30;
  scene1.camPosition.z = -40;
  scene1.camRotY = 0.00;
  // scene1.camRotY = 1.00;

  // scene1.objects.back().currentVel = glm::vec3(0.0,16.0,40.0);
  scene1.objects.back().currentVel = glm::vec3(0.0,0.0,0.0);

  for (auto &obj : scene1.objects) {
    obj.gravG = -9.81;
    obj.gravMass = 10.0*rand()/(float)RAND_MAX + 2.0;
    obj.physGrav = false;
    obj.doColl = false;
  }

  scene1.objects.back().doColl = true;
  scene1.objects.back().physGrav = false;

  scene1.objects.back().gravMass = 1000.0;
    
  return 0;
}

void MySGLEngine::Render() {
  std::cout << "Starting rendering thread..." << std::endl;
  std::thread t1([&] { this->RenderScene(scene1); });

 // std::cout << "Starting physics thread..." << std::endl;
 // std::thread t2([&] { this->Physics(scene1); });

  std::cout << "Waiting for threads..." << std::endl;
  t1.join();
  std::cout << "Rendering thread ended!" << std::endl;
  //t2.join();
  //std::cout << "Physics thread ended!" << std::endl;
}

static void key_callback_user(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  e.UserKeyHandling(key, action, mods);
}  

static void mouse_callback_user(GLFWwindow* window, int button, int action, int mods) {
  e.UserMouseHandling(button, action, mods);
}  

static void mouse_move_callback_user(GLFWwindow* window, double xpos, double ypos) {
  e.UserMouseMoveHandling(xpos, ypos);
}  

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "./test.x object" << std::endl;
    exit(1);
  }

  scenefilename = argv[1];

  e.Init();
  e.UserInit();

  std::cout << "Setting up scene..." << std::endl;
  if (e.SetupScene() != 0)
    return 1;

  std::cout << "Starting main loop..." << std::endl;
  e.Run();
}
