
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

class MySGLEngine : public SGLEngine {
  public:
    int SetupScene();

  private:
    void Render();
};

int MySGLEngine::SetupScene() {
  SGLEngine::Object obj1;

  std::string filepath(objectfilename);
  if(ObjParser(filepath, obj1) != 0)
    std::cout << "Error loading object " << filepath << " !" << std::endl;

  glfwMakeContextCurrent(window);

  for(int i=0; i<obj1.vertices.size(); i++) {
    obj1.colors.push_back(rand()/(double)RAND_MAX);
  }
  
  /* obj1.uvs.resize(0);
  for(int i=0; i<obj1.vertices.size()/3*2; i++) {
    obj1.uvs.push_back(rand()/(double)RAND_MAX);
  } */

  obj1.isIndexed = true;
  
  SetupObject(obj1);
  obj1.shader = SGLEngine::LoadShaders( vertshaderfilename.c_str(), NULL, fragshaderfilename.c_str() );
  scene1.objects.push_back(obj1);

  return 0;
}

void MySGLEngine::Render() {
  std::cout << "Rendering..." << std::endl;
  
  glfwMakeContextCurrent(window);

  static float time=-1;
  static float delta=0.005;

  while (!glfwWindowShouldClose(window)) {   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection matrix : 70° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(70.0f, 4.0f / 4.0f, 0.1f, 100.0f);
    // Camera matrix
     glm::mat4 View = glm::lookAt(
         glm::vec3(4,2,-3), // Camera is at (4,3,3), in World Space
         glm::vec3(0,0,0), // and looks at the origin
         glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), (float)(time*2.0*M_PI), glm::vec3(0.0f, 1.0f, 0.0f));

    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

    for(int i=0; i<scene1.objects.size(); i++) {
      glBindVertexArray(scene1.objects[i].vaoid[0]);
      glUseProgram(scene1.objects[i].shader);
      
      GLuint MatrixID = glGetUniformLocation(scene1.objects[i].shader, "MVP");
      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

      GLuint TextureID  = glGetUniformLocation(scene1.objects[i].shader, "myTextureSampler");
      glUniform1i(TextureID, 0);

      if (scene1.objects[i].isIndexed) {
        glDrawElements(GL_TRIANGLES, scene1.objects[i].indices.size(), GL_UNSIGNED_INT, 0);
      } else {
        glDrawArrays(GL_TRIANGLES, 0, scene1.objects[i].vertices.size()/3); 
      }

      GLenum glerr;
      while ((glerr = glGetError()) != GL_NO_ERROR)
        std::cerr << "OpenGL error: " << glerr << std::endl;
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    
    time += delta;
    if (time > 1) { time = 0;}
  }   
}

int main(int argc, char *argv[]) {

  if (argc < 4) {
    std::cout << "./test.x object vertexshader fragmentshader" << std::endl;
    exit(1);
  }

  objectfilename = argv[1];
  vertshaderfilename = argv[2];
  fragshaderfilename = argv[3];

  MySGLEngine e;

  e.Init();

  std::cout << "Setting up scene..." << std::endl;
  e.SetupScene();

  std::cout << "Starting main loop..." << std::endl;
  e.Run();
}
