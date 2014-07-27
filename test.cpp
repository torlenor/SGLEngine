#include <iostream>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif

// #include <GL/glext.h>

#include "sglengine.h"

class MySGLEngine : public SGLEngine {
  public:
    int SetupScene();
    void Run();
  private:
    Object obj1;
    GLuint *VAOs;
    GLuint *Buffers;
    GLuint programID;
    void Render();
};

int MySGLEngine::SetupScene() {
  std::string filepath("monkey.obj");
  if(ObjParser(filepath, obj1) != 0)
    std::cout << "Error loading object " << filepath << " !" << std::endl;

  glfwMakeContextCurrent(window);

  /* glewExperimental = GL_TRUE; 
  GLenum err=glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return -1;
  } */

  // VAOs =  new GLuint[1];
  // Buffers = new GLuint[1];
  
  GLuint NumBuffers=1;
  GLuint NumVAOs=1;

  GLuint VAOs[NumVAOs];
  GLuint Buffers[NumBuffers];

  GLuint NumVertices = 3;
  
  GLfloat vertices[3][2] = { 
      { -0.90, -0.90 }, // Triangle 1
      { 0.85, -0.90 },
      { -0.90, 0.85 },
    };  
  
  std::cout << "Generating VAOs..." << std::endl;
  glGenVertexArrays(1, VAOs);
  glBindVertexArray(VAOs[0]);
  
  //GLenum glerr;
  //while ((glerr = glGetError()) != GL_NO_ERROR)
  //  std::cerr << "OpenGL error: " << glerr << std::endl;

  std::cout << "Copy buffers..." << std::endl;
  glGenBuffers(1, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*2, vertices, GL_STATIC_DRAW);

  std::cout << "Loading shaders..." << std::endl;
  programID = LoadShaders( "./shaders/simple.vs", "./shaders/simple.fs");
  glUseProgram(programID);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(0);  

  return 0;
}

void MySGLEngine::Render() {
  std::cout << "Rendering..." << std::endl;
  
  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window))
  {   
      glClear(GL_COLOR_BUFFER_BIT);
  
      glLoadIdentity();
      // glMatrixMode(GL_MODELVIEW);
      // glTranslatef(0,0,-3.0);
      // glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
     
      // glUseProgram(programID);

      glEnableVertexAttribArray(0);
      glBindVertexArray(VAOs[0]);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

      glDrawArrays(GL_TRIANGLES, 0, 3);

      glBindVertexArray(0); // Unbind our Vertex Array Object 
      glDisableVertexAttribArray(0);

      glfwSwapBuffers(window);
      glfwPollEvents();
  }   
}

void MySGLEngine::Run() {

  while (!glfwWindowShouldClose(window))
    Render();
}

int main() {
  MySGLEngine e;

  e.Init();


  std::cout << "Setting up scene..." << std::endl;
  e.SetupScene();

  std::cout << "Starting main loop..." << std::endl;
  e.Run();
}
