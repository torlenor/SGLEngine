#include <iostream>
#include <thread>

#include <GL/glew.h>
#include <GL/gl.h>
// #include <GL/glext.h>
#include <GLFW/glfw3.h>

#include "sglengine.h"

class MySGLEngine : public SGLEngine {
  public:
    int SetupScene();
  private:
    Object obj1;
    GLuint *VAOs;
    GLuint *Buffers;
    void Render();
};

int MySGLEngine::SetupScene() {
  std::string filepath("monkey.obj");
  if(ObjParser(filepath, obj1) != 0)
    std::cout << "Error loading object " << filepath << " !" << std::endl;

  glfwMakeContextCurrent(window);

  GLenum err=glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return -1;
  }

  VAOs = new GLuint[1];
  Buffers = new GLuint[1];

  const GLuint NumVertices = 6;

  std::cout << "Generating VAOs..." << std::endl;
  glGenVertexArrays(1, VAOs);
  glBindVertexArray(VAOs[0]);
  
  GLfloat vertices[NumVertices][2] = { 
      { -0.90, -0.90 }, // Triangle 1
      { 0.85, -0.90 },
      { -0.90, 0.85 },
      { 0.90, -0.85 }, // Triangle 2
      { 0.90, 0.90 },
      { -0.85, 0.90 }
    };  

  std::cout << "Copy buffers..." << std::endl;
  glGenBuffers(1, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //GLuint programID = LoadShaders( "./shaders/simple.vs", "./shaders/simple.fs");
  // glUseProgram(programID);

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
    
        glMatrixMode(GL_MODELVIEW);

        glTranslatef(0,0,-3.0);
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6); 
        glFinish();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }   
}

int main() {
  MySGLEngine e;

  e.Init();


  std::cout << "Setting up scene..." << std::endl;
  e.SetupScene();

  std::cout << "Starting main loop..." << std::endl;
  e.Run();
}
