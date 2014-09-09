
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

std::string scenefilename;

SGLEngine::Scene scene1;

static void key_callback_user(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback_user(GLFWwindow* window, int key, int action, int mods);

void BuildWalls(SGLEngine::Object &obj);

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
    obj.currentVel = 4.0f*glm::vec3(-1.6*(rand()/(float)RAND_MAX-0.5), 1.4*(rand()/(float)RAND_MAX-0.5), 0.0);
    scene1.objects.push_back(obj);
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

void MySGLEngine::UserKeyHandling(int key, int action, int mods) {
  if (key == GLFW_KEY_W && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = 15.0;
  if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_S && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = -15.0;
  if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = 15.0;
  if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_E && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = -15.0;
  if (key == GLFW_KEY_E && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_X && action == GLFW_PRESS)
    scene1.deltaCamPosition.y = -15.0;
  if (key == GLFW_KEY_X && action == GLFW_RELEASE)
    scene1.deltaCamPosition.y = 0.0;
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    scene1.deltaCamPosition.y = 15.0;
  if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    scene1.deltaCamPosition.y = 0.0;
  
  if (key == GLFW_KEY_A && action == GLFW_PRESS)
    scene1.deltaCamRotY = 1.5*M_PI/(float)2.0;
  if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    scene1.deltaCamRotY = 0.0;
  
  if (key == GLFW_KEY_D && action == GLFW_PRESS)
    scene1.deltaCamRotY = -1.5*M_PI/(float)2.0;
  if (key == GLFW_KEY_D && action == GLFW_RELEASE)
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
}

void MySGLEngine::UserInit() {
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback_user);
  glfwSetMouseButtonCallback(window, mouse_callback_user);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  glfwSetWindowTitle(window, "My OpenGL Engine");
}

int MySGLEngine::SetupScene() {

  glfwMakeContextCurrent(window);

  // std::string scenefile("scene.data");
  if ( LoadScene(scene1, scenefilename) != 0) 
    return 1;

  scene1.camPosition.x = -50;
  scene1.camPosition.y = 30;
  scene1.camPosition.z = -40;
  scene1.camRotY = 0.969715;

  scene1.objects.back().currentVel = glm::vec3(0.0,8.0,30.0);

  for (auto &obj : scene1.objects) {
    obj.gravG=-9.81;
    obj.gravMass=10.0*rand()/(float)RAND_MAX + 2.0;
    obj.physGrav=false;
  }

  scene1.objects.back().gravMass = 100.0;
    
  return 0;
}

void Colission() {
  glm::vec3 currentVel = glm::vec3(0.0f,0.0f,0.0f);
  static std::vector<bool> colHappened(scene1.objects.size(),false);
  float k=0.1;
  if ( scene1.objects.back().currentPos.z + scene1.objects.back().boundingBox[4]*scene1.objects.back().scale.z > -scene1.objects[0].boundingBox[4]) {
    // colHappened = true;
      float carMass = scene1.objects.back().gravMass;
      float carVelx = scene1.objects.back().currentVel.x;
      float carVely = scene1.objects.back().currentVel.y;
      float carVelz = scene1.objects.back().currentVel.z;

    for (int i=0; i<(int)scene1.objects.size()-2; i++) {
      float objMass = scene1.objects[i].gravMass;
      float objVelx = scene1.objects[i].currentVel.x;
      float objVely = scene1.objects[i].currentVel.y;
      float objVelz = scene1.objects[i].currentVel.z;
      if ( scene1.objects[i].currentPos.x + scene1.objects[i].boundingBox[1] < (scene1.objects.back().currentPos.x + scene1.objects.back().boundingBox[0]*scene1.objects.back().scale.x)
          && scene1.objects[i].currentPos.x + scene1.objects[i].boundingBox[0] > scene1.objects.back().currentPos.x + scene1.objects.back().boundingBox[1]*scene1.objects.back().scale.x
          && scene1.objects[i].currentPos.y + scene1.objects[i].boundingBox[3] < scene1.objects.back().currentPos.y + scene1.objects.back().boundingBox[2]*scene1.objects.back().scale.y
          && scene1.objects[i].currentPos.y + scene1.objects[i].boundingBox[2] > scene1.objects.back().currentPos.y + scene1.objects.back().boundingBox[3]*scene1.objects.back().scale.y
          && scene1.objects[i].currentPos.z + scene1.objects[i].boundingBox[5] < scene1.objects.back().currentPos.z + scene1.objects.back().boundingBox[4]*scene1.objects.back().scale.z
          && scene1.objects[i].currentPos.z + scene1.objects[i].boundingBox[4] > scene1.objects.back().currentPos.z + scene1.objects.back().boundingBox[5]*scene1.objects.back().scale.z
          && !colHappened[i]
         ) {

        colHappened[i] = true;

        float newcarVelx = (carMass*carVelx
                           + objMass*objVelx
                           - objMass*(carVelx - objVelx)*k)
                            / (carMass + objMass);
        
        float newcarVely = (carMass*carVely
                           + objMass*objVely
                           - objMass*(carVely - objVely)*k)
                            / (carMass + objMass);
        
        float newcarVelz = (carMass*carVelz
                           + objMass*objVelz
                           - objMass*(carVelz - objVelz)*k)
                            / (carMass + objMass);

        scene1.objects.back().currentVel.x = newcarVelx;
        scene1.objects.back().currentVel.y = newcarVely;
        scene1.objects.back().currentVel.z = newcarVelz;
        scene1.objects.back().physGrav=true;

        float newobjVelx = (carMass*carVelx
                           + objMass*objVelx
                           - carMass*(objVelx - carVelx)*k)
                            / (carMass + objMass);
        
        float newobjVely = (carMass*carVely
                           + objMass*objVely
                           - carMass*(objVely - carVely)*k)
                            / (carMass + objMass);
        
        float newobjVelz = (carMass*carVelz
                           + objMass*objVelz
                           - carMass*(objVelz - carVelz)*k)
                            / (carMass + objMass);
        
        currentVel = glm::vec3( newobjVelx,
                                newobjVely,
                                newobjVelz
                              );

        scene1.objects[i].currentVel = currentVel;
        scene1.objects[i].physGrav=true;
        scene1.objects[i].currentRot = glm::vec3((rand()/(float)RAND_MAX-0.5)*45.0f, (rand()/(float)RAND_MAX-0.5)*45.0f, (rand()/(float)RAND_MAX-0.5)*45.0f);
        
        std::cout << i << " " << objMass << std::endl;
      }
    }
  }

  for (unsigned int i=0; i<scene1.objects.size(); i++) {
    if (scene1.objects[i].currentPos.y < 0.0f && i != scene1.objects.size() - 2 ) {
      scene1.objects[i].physGrav=false;
      scene1.objects[i].currentVel = glm::vec3(0.0f, 0.0f, 0.0f);
      // scene1.objects[i].currentPos.y = 0.0f;
    }
  }
}

void ChangeSomeStuff() {
  // std::cout << "ChangeSomeStuff() called!" << std::endl;
  float cubesize = 58.0f;
  SGLEngine::Object &obj = scene1.objects.back();
    glm::vec3 currentPos = obj.currentPos;
    glm::vec3 currentVel = obj.currentVel;
    if (currentPos.x > cubesize) {
      currentVel.x *= -1.0;
      currentPos.x = cubesize;
      obj.currentPos = currentPos;
      obj.currentVel = currentVel;
    }
    if (currentPos.x < 0) {
      currentVel.x *= -1.0;
      currentPos.x = 0.0;
      obj.currentPos = currentPos;
      obj.currentVel = currentVel;
    }
    if (currentPos.y > cubesize) {
      currentVel.y *= -1.0;
      currentPos.y = cubesize;
      obj.currentPos = currentPos;
      obj.currentVel = currentVel;
    }
    if (currentPos.y < -0) {
      currentVel.y *= -1.0;
      currentPos.y = 0.0;
      obj.currentPos = currentPos;
      obj.currentVel = currentVel;
    }
    if (currentPos.z < -cubesize) {
      currentVel.z *= -1.0;
      currentPos.z = -cubesize;
      obj.currentPos = currentPos;
      obj.currentVel = currentVel;
    }
    if (currentPos.z + 2.0 > 0) {
      currentVel.z *= -1.0;
      currentPos.z = -2.0;
      obj.currentPos = currentPos;
      obj.currentVel = currentVel;
    }
}

void MySGLEngine::Render() {
  std::cout << "Starting rendering thread..." << std::endl;
  std::thread t1([&] { this->RenderScene(scene1); });

  struct timespec tim1;
  tim1.tv_sec=0;
  tim1.tv_nsec=50*(1000000);
  while (!glfwWindowShouldClose(window)) {
    // ChangeSomeStuff();
    Colission();
    nanosleep(&tim1, NULL);
  }
  
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

void BuildWalls(SGLEngine::Object &obj) {
  float size=58.0f; // in every direction from 0,0,0

  std::vector<float> vertices, colors, normals;
  std::vector<GLuint> indices;

  GLuint triangles=0;
  int Npoints=2;
  for(int x=0; x<Npoints; x++) {
    float xval = 4.0f*size*x/(float)Npoints - size;
    for(int y=0; y<Npoints; y++) {
      float yval = 4.0f*size*y/(float)Npoints - size;
      for(int z=0; z<Npoints; z++) {
        float zval = 4.0f*size*z/(float)Npoints - size;

        // We need to generate the triangle now
        glm::vec3 p1(xval, yval, zval);
        glm::vec3 p2(0,0,0);
        glm::vec3 p3(0,0,0);
        vertices.push_back(p1.x); vertices.push_back(p1.y); vertices.push_back(p1.z);
        vertices.push_back(p2.x); vertices.push_back(p2.y); vertices.push_back(p2.z);
        vertices.push_back(p3.x); vertices.push_back(p3.y); vertices.push_back(p3.z);
        
        // Build normal vector using cross product vec3 = glm::cross(vec3,vec3)
        glm::vec3 v1 = p2 - p1;
        glm::vec3 v2 = p3 - p2;
        glm::vec3 v3 = p1 - p3;
        glm::vec3 normv = glm::cross(v1,v3);
        normals.push_back(normv.x); normals.push_back(normv.y); normals.push_back(normv.z);
        normv = glm::cross(v1,v2);
        normals.push_back(normv.x); normals.push_back(normv.y); normals.push_back(normv.z);
        normv = glm::cross(v2,v3);
        normals.push_back(normv.x); normals.push_back(normv.y); normals.push_back(normv.z);

        // Give them a color
        colors.push_back((float)x);
        colors.push_back((float)y);
        colors.push_back((float)z);
        colors.push_back((float)x);
        colors.push_back((float)y);
        colors.push_back((float)z);
        colors.push_back((float)x);
        colors.push_back((float)y);
        colors.push_back((float)z);
        
        // Build the index for the triangle
        indices.push_back(triangles); // 1st vertex
        indices.push_back(triangles+1); // 2nd vertex
        indices.push_back(triangles+2); // 3rd vertex

        triangles++;
      }
    }
  }

  obj.vertices = vertices;
  obj.colors = colors;
  obj.normals = normals;
  obj.indices = indices;
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
