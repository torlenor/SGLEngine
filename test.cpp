
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
    scene1.deltaCamPosition.z = 10.0;
  if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_S && action == GLFW_PRESS)
    scene1.deltaCamPosition.z = -10.0;
  if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    scene1.deltaCamPosition.z = 0.0;
  
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = 10.0;
  if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_E && action == GLFW_PRESS)
    scene1.deltaCamPosition.x = -10.0;
  if (key == GLFW_KEY_E && action == GLFW_RELEASE)
    scene1.deltaCamPosition.x = 0.0;
  
  if (key == GLFW_KEY_X && action == GLFW_PRESS)
    scene1.deltaCamPosition.y = -10.0;
  if (key == GLFW_KEY_X && action == GLFW_RELEASE)
    scene1.deltaCamPosition.y = 0.0;
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    scene1.deltaCamPosition.y = 10.0;
  if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    scene1.deltaCamPosition.y = 0.0;
  
  if (key == GLFW_KEY_A && action == GLFW_PRESS)
    scene1.deltaCamRotY = 2.0*M_PI/(float)2.0;
  if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    scene1.deltaCamRotY = 0.0;
  
  if (key == GLFW_KEY_D && action == GLFW_PRESS)
    scene1.deltaCamRotY = -2.0*M_PI/(float)2.0;
  if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    scene1.deltaCamRotY = 0.0;
  
  if (key == GLFW_KEY_R && action == GLFW_PRESS)
    scene1.deltaCamRotZ = 2.0*M_PI/(float)2.0;
  if (key == GLFW_KEY_R && action == GLFW_RELEASE)
    scene1.deltaCamRotZ = 0.0;
  
  if (key == GLFW_KEY_F && action == GLFW_PRESS)
    scene1.deltaCamRotZ = -2.0*M_PI/(float)2.0;
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

  glfwSetWindowTitle(window, "My OpenGL Engine");
}

int MySGLEngine::SetupScene() {
  SGLEngine::Object obj1;

  std::string filepath(objectfilename);
  if (ObjParser(filepath, obj1) != 0)
    std::cout << "Error loading object " << filepath << " !" << std::endl;

  glfwMakeContextCurrent(window);

  for(unsigned int i=0; i<obj1.vertices.size(); i++) {
    obj1.colors.push_back(rand()/(double)RAND_MAX);
  }
   
  obj1.isIndexed = true;
  SetupObject(obj1);
  obj1.shader = SGLEngine::LoadShaders( vertshaderfilename.c_str(), NULL, fragshaderfilename.c_str() );
  obj1.scale = glm::vec3(1.0,1.0,1.0);

  // Wall in front of me
  obj1.currentPos = glm::vec3(0.0, 0.0, 10.0);
  scene1.objects.push_back(obj1);
  obj1.currentPos = glm::vec3(5.0, 5.0, 10.0);
  scene1.objects.push_back(obj1);
  obj1.currentPos = glm::vec3(5.0, -5.0, 10.0);
  scene1.objects.push_back(obj1);
  obj1.currentPos = glm::vec3(-5.0, -5.0, 10.0);
  scene1.objects.push_back(obj1);
  obj1.currentPos = glm::vec3(-5.0, 5.0, 10.0);
  scene1.objects.push_back(obj1);
  
  // Wall behind me
  obj1.currentPos = glm::vec3(0.0, 0.0, -10.0);
  scene1.objects.push_back(obj1);
  obj1.currentPos = glm::vec3(5.0, 5.0, -10.0);
  scene1.objects.push_back(obj1);
  obj1.currentPos = glm::vec3(5.0, -5.0, -10.0);
  scene1.objects.push_back(obj1);
  obj1.currentPos = glm::vec3(-5.0, -5.0, -10.0);
  scene1.objects.push_back(obj1);
  obj1.currentPos = glm::vec3(-5.0, 5.0, -10.0);
  scene1.objects.push_back(obj1);
  
  // Sides
  obj1.currentPos = glm::vec3(-10.0, 0.0, 0.0);
  scene1.objects.push_back(obj1);
  
  // Sides
  obj1.currentPos = glm::vec3(10.0, 0.0, 0.0);
  scene1.objects.push_back(obj1);
  
  // Top
  obj1.currentPos = glm::vec3(0.0, 10.0, 0.0);
  scene1.objects.push_back(obj1);

  // Give one cube a velocity
  for (unsigned int i=0; i<scene1.objects.size()-1; i++) {
    scene1.objects[i].currentVel = 4.0f*glm::vec3(-1.6*(rand()/(float)RAND_MAX-0.5), 1.4*(rand()/(float)RAND_MAX-0.5), 0.0);
  }
  // scene1.objects[0].currentVel = glm::vec3(-1.6, 1.4, 0.0);
  
  Object obj2;
  BuildWalls(obj2);
  obj2.isIndexed = true;
  obj2.scale = glm::vec3(20.0,20.0,20.0);
  SetupObject(obj2);
  obj2.shader = SGLEngine::LoadShaders( vertshaderfilename.c_str(), NULL, "shaders/simple_color.fs" );
  scene1.objects.push_back(obj2);
  
  InfoObject(obj2);

  scene1.camPosition = glm::vec3(0.0f, 0.0f, 0.0f);

  // Build a Wall
  SGLEngine::Object obj_wall;

  filepath = std::string("obj/walls.obj");
  if (ObjParser(filepath, obj_wall) != 0)
    std::cout << "Error loading object " << filepath << " !" << std::endl;
  
  obj_wall.isIndexed = true;
  obj_wall.scale = glm::vec3(10.0, 10.0, 10.0);
  obj_wall.colors.resize(obj_wall.vertices.size());
  for(unsigned int i=0; i<obj_wall.colors.size(); i += 3)
    obj_wall.colors[i] = 1.0;
  obj_wall.currentPos = glm::vec3(0.0, 0.0, 0.0);
  obj_wall.shader = SGLEngine::LoadShaders( vertshaderfilename.c_str(), NULL, "shaders/simple_color.fs" );
  SetupObject(obj_wall);
  // obj_wall.shader = SGLEngine::LoadShaders( vertshaderfilename.c_str(), NULL, "shaders/simple_color.fs" );
  
  InfoObject(obj_wall);

  scene1.objects.push_back(obj_wall);

  return 0;
}

void ChangeSomeStuff() {
  // std::cout << "ChangeSomeStuff() called!" << std::endl;
  float cubesize = 20.0f;
  for(unsigned int i=0; i<scene1.objects.size(); i++) {
    glm::vec3 currentPos = scene1.objects[i].currentPos;
    glm::vec3 currentVel = scene1.objects[i].currentVel;
    if (currentPos.x + 1.0> cubesize) {
      currentVel.x *= -1.0;
      currentPos.x = cubesize - 1.0;
      scene1.objects[i].currentPos = currentPos;
      scene1.objects[i].currentVel = currentVel;
    }
    if (currentPos.x - 1.0 < -cubesize) {
      currentVel.x *= -1.0;
      currentPos.x = -cubesize + 1.0;
      scene1.objects[i].currentPos = currentPos;
      scene1.objects[i].currentVel = currentVel;
    }
    if (currentPos.y + 1.0 > cubesize) {
      currentVel.y *= -1.0;
      currentPos.y = cubesize - 1.0;
      scene1.objects[i].currentPos = currentPos;
      scene1.objects[i].currentVel = currentVel;
    }
    if (currentPos.y - 1.0 < -cubesize) {
      currentVel.y *= -1.0;
      currentPos.y = -cubesize + 1.0;
      scene1.objects[i].currentPos = currentPos;
      scene1.objects[i].currentVel = currentVel;
    }
  }

/*  SGLEngine::Object obj = scene1.objects[1];
  obj.currentPos = glm::vec3(2.0*rand()/(float)RAND_MAX - 1.0, 2.0*rand()/(float)RAND_MAX - 1.0, 2.0*rand()/(float)RAND_MAX - 1.0)*10.0f;
  scene1.objects.push_back(obj); */
}

void MySGLEngine::Render() {
  std::cout << "Starting rendering thread..." << std::endl;
  std::thread t1([&] { this->RenderScene(scene1); });

  InfoObject(scene1.objects[0]);

  struct timespec tim1;
  tim1.tv_sec=0;
  tim1.tv_nsec=100*(1000000);
  // tim1.tv_nsec=0;
  while (!glfwWindowShouldClose(window)) {
    ChangeSomeStuff();
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
  float size=20.0f; // in every direction from 0,0,0

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
