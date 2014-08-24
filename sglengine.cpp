/*
 * sglengine.cpp - sglengine class
 *
 * Copyright © 2014 H.-P. Schadler  <hps@abyle.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "sglengine.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>

#include "loadbmp.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

SGLEngine::SGLEngine() {

}

SGLEngine::~SGLEngine() {

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}   

static void error_callback(int error, const char* description) {
  fputs(description, stderr);
}

int SGLEngine::Init() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);
  

  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(800, 800, "Simple example", NULL, NULL);
  if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  
  glfwSetKeyCallback(window, key_callback);

  glewExperimental = GL_TRUE;
  if( GLEW_OK != glewInit() )
    exit(EXIT_FAILURE);

  fputs("Window Created\n", stdout);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  
  // glEnable(GL_CULL_FACE);

  return 0;
}

void SGLEngine::InfoObject(SGLEngine::Object &obj) {
  std::cout << "Object Info:\n"
    << "Number of vertices = " << obj.vertices.size()/3 << "\n" 
    << "Number of colors = " << obj.colors.size()/3 << "\n" 
    << "Number of normals = " << obj.normals.size()/3 << "\n" 
    << "Number of UVs = " << obj.uvs.size()/2 << "\n" 
    << "Number of indices = " << obj.indices.size() << "\n" 
    << "VAOid = " << obj.vaoid[0] << "\n" 
    << "Shader number = " << obj.shader << "\n" 
    << "Current position = (" << obj.currentPos.x << "," << obj.currentPos.y << "," << obj.currentPos.z << ") \n" 
    << "Current velocity = (" << obj.currentVel.x << "," << obj.currentVel.y << "," << obj.currentVel.z << ") \n" 
    << "END Object info" << std::endl;
}

int SGLEngine::SetupObject(SGLEngine::Object &obj) {
  glGetError();

  std::cout << "Generating VAOs..." << std::endl;
  obj.vaoid.resize(1);

  glGenVertexArrays(1, &obj.vaoid[0]);
  glBindVertexArray(obj.vaoid[0]);
  
  std::cout << "Copy buffers..." << std::endl;
  obj.bufferid.resize(1);
  glGenBuffers(1, &obj.bufferid[0]);

  glBindBuffer(GL_ARRAY_BUFFER, obj.bufferid[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*obj.vertices.size(), &obj.vertices[0], GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*obj.vertices.size(), &obj.vertices[0], GL_DYNAMIC_DRAW);

  // Indices //
  if (obj.isIndexed) {
    glGenBuffers(1, &obj.indid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.indid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indices.size() * sizeof(GLuint),
        &obj.indices[0], GL_STATIC_DRAW);
  }
  // INDICES END //
  
  // Vertices //
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.indid);
  glVertexAttribPointer(
      0,        // attribute. No particular reason for 1, but must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void*)0  // array buffer offset
  );
  // Vertices END //

  // COLOR //
  GLuint colorbuffer;
  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*obj.colors.size(), &obj.colors[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer(
      1,        // attribute. No particular reason for 1, but must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void*)0  // array buffer offset
  );
  // COLOR END //
  
  // NORMALS //
  GLuint normalbuffer;
  glGenBuffers(1, &normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*obj.normals.size(), &obj.normals[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glVertexAttribPointer(
      2,        // attribute. No particular reason for 1, but must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void*)0  // array buffer offset
  );
  // NORMALS END //
  
  // UVS //
  if (obj.usesUVs) {
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*obj.uvs.size(), &obj.uvs[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        3,        // attribute. No particular reason for 1, but must match the layout in the shader.
        2,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void*)0  // array buffer offset
    );
  }
  // UVS END //

  // TEXTURE //
  const char *imagepath="textures/cubetex1.bmp";
  // const char *imagepath="textures/p51mustang.bmp";
  // const char *imagepath="textures/uvtemplate.bmp";
  GLuint texID=0;
  texID=loadBMP_custom(imagepath);

  // TEXTURE END //

  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

  return 0;
}

int SGLEngine::SetupScene() {
  
  return 0;
}

void SGLEngine::Run() {
  while (!glfwWindowShouldClose(window))
    Render();
}

void SGLEngine::Render() {
  // has to be implemented in derived class
}

void SGLEngine::PrintFPS() {
  static int g_measurecount=-1;
  static double g_averagems=0;
  static double timebase=0;
  static double ttime=0;
  static int frame=0;

  ttime = glfwGetTime();

  frame++;

  int ns=1.0;
  if (ttime - timebase > ns) {
    if(g_measurecount != -1){
      g_averagems += 1000.0*(ttime-timebase)/(double)frame;
    }
    g_measurecount++;

    printf("FPS = %4.2f; Average time to render frame = %4.2f ms\n",
      1.0*frame*(double)ns/(ttime-timebase),
      1000.0*(ttime-timebase)/(double)frame);
    printf("Total average time to render frame: %4.2f ms\n", g_averagems/(double)g_measurecount);
    timebase = ttime;
    frame = 0;
  }
}

void Explode(SGLEngine::Object &obj, const double deltaTime) {
    float vertChange = 5.0*deltaTime;
    int rnd1 = rand() % (obj.vertices.size());
    if(rand()/(float)RAND_MAX < 0.5) {
      obj.vertices[rnd1] += vertChange;
    } else {
      obj.vertices[rnd1] -= vertChange;
    }

    glBindBuffer(GL_ARRAY_BUFFER, obj.bufferid[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*obj.vertices.size(), &obj.vertices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SGLEngine::UpdateCamera(SGLEngine::Scene &scene, const double deltaTime) {
    scene.camRotY += scene.deltaCamRotY*(float)(deltaTime);
    scene.camRotZ += scene.deltaCamRotZ*(float)(deltaTime);
    scene.camPosition.x += cos(scene.camRotY)*scene.deltaCamPosition.x*(float)deltaTime
                          +sin(scene.camRotY)*scene.deltaCamPosition.z*(float)deltaTime;
    scene.camPosition.y += scene.deltaCamPosition.y*(float)deltaTime;
    scene.camPosition.z += -sin(scene.camRotY)*scene.deltaCamPosition.x*(float)deltaTime
                          +cos(scene.camRotY)*scene.deltaCamPosition.z*(float)deltaTime;

    if (scene.camRotY > 2.0*M_PI) scene.camRotY -= 2.0*M_PI;
    if (scene.camRotY < 0) scene.camRotY += 2.0*M_PI;
    
    if (scene.camRotZ > M_PI/2.0f) scene.camRotZ = M_PI/2.0f;
    if (scene.camRotZ < -M_PI/2.0f) scene.camRotZ = -M_PI/2.0f;
}

void SGLEngine::RenderScene(SGLEngine::Scene &scene) {
  glfwMakeContextCurrent(window);

  static float time = 0;
  static float delta = 0.2;

  double oldtime=glfwGetTime();
  double newtime=0;
  double deltaTime=0;

  while (!glfwWindowShouldClose(window)) {
    newtime = glfwGetTime();
    deltaTime = newtime - oldtime;
    oldtime = newtime;
    
    time += delta*(deltaTime);
    if (time > 1) { time = 0; } 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    ratio = width/(float)height;
    ratio = ratio;

    UpdateCamera(scene, deltaTime);

    // FOVY is in radiant
    glm::mat4 Projection = glm::perspective((float)(70.0f/360.0f*2.0f*M_PI), ratio, 0.1f, 100.0f);

    glm::mat4 View = glm::lookAt(
      scene.camPosition, // Camera in World Space
      scene.camPosition + glm::vec3(sin(scene.camRotY), sin(scene.camRotZ), cos(scene.camRotY)), // and look at
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // for(int i=0; i<scene.objects.size(); i++) {
    for(auto &obj : scene.objects) {
      obj.currentPos += obj.currentVel*(float)deltaTime;

      glm::mat4 Model = glm::translate(glm::mat4(1.0f), obj.currentPos);
      Model = glm::scale(Model, obj.scale);
      // Model = glm::rotate(Model, (float)(time*2.0*M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
      // Model = glm::rotate(Model, (float)(time*2.0*M_PI), glm::vec3(1.0f, 1.0f, 0.0f));

      glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

      glBindVertexArray(obj.vaoid[0]);
      glUseProgram(obj.shader);
    
      GLuint MatrixID = glGetUniformLocation(obj.shader, "MVP");
      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

      GLuint TextureID  = glGetUniformLocation(obj.shader, "myTextureSampler");
      glUniform1i(TextureID, 0); 

      if (obj.isIndexed) {
        glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT, 0); 
      } else {
        glDrawArrays(GL_TRIANGLES, 0, obj.vertices.size()/3); 
      }   

      GLenum glerr;
      while ((glerr = glGetError()) != GL_NO_ERROR)
        std::cerr << "OpenGL error: " << glerr << std::endl;
    }
  
    // Try an update of the vertices
    // Explode(scene.objects[0], deltaTime);

    glfwSwapBuffers(window);
    glfwPollEvents();

    PrintFPS();

  }
}

void SGLEngine::CheckStatus( GLuint obj ) { 
  GLint status = GL_FALSE, len = 10; 
  if( glIsShader(obj) )   glGetShaderiv( obj, GL_COMPILE_STATUS, &status );
  if( glIsProgram(obj) )  glGetProgramiv( obj, GL_LINK_STATUS, &status );
  if( status == GL_TRUE ) return;
  if( glIsShader(obj) )   glGetShaderiv( obj, GL_INFO_LOG_LENGTH, &len );
  if( glIsProgram(obj) )  glGetProgramiv( obj, GL_INFO_LOG_LENGTH, &len );
  std::vector<char> log( len, 'X' );
  if( glIsShader(obj) )   glGetShaderInfoLog( obj, len, NULL, &log[0] );
  if( glIsProgram(obj) )  glGetProgramInfoLog( obj, len, NULL, &log[0] );
  std::cerr << &log[0] << std::endl;
  exit( -1 );
}

void SGLEngine::AttachShader( GLuint program, GLenum type, const char* src ) { 
  GLuint shader = glCreateShader( type );
  glShaderSource( shader, 1, &src, NULL );
  glCompileShader( shader );
  CheckStatus( shader );
  glAttachShader( program, shader );
  glDeleteShader( shader );
}  

GLuint SGLEngine::LoadShaders(const char* vertFileName, const char* geomFileName, const char* fragFileName) { 
  GLuint prog = glCreateProgram();

  std::stringstream vertss, geomss, fragss;
  std::string verts, geoms, frags;
  std::ifstream shaderfile;
  // Load the shaders
  if (vertFileName) {
    shaderfile.open(vertFileName);
    if (shaderfile.is_open()) {
      vertss << shaderfile.rdbuf();
      verts = vertss.str();
      shaderfile.close();
    } else {
      std::cerr << "ERROR: Problem open vetrex shader file: " << vertFileName << std::endl;
      exit(1);
    }
    const char *vert = verts.c_str();
    AttachShader(prog, GL_VERTEX_SHADER, vert);
  }

  if (geomFileName) {
    shaderfile.open(geomFileName);
    if (shaderfile.is_open()) {
      geomss << shaderfile.rdbuf();
      geoms = geomss.str();
      shaderfile.close();
    } else {
      std::cerr << "ERROR: Problem open geometry shader file: " << geomFileName << std::endl;
      exit(1);
    }
    const char *geom = geoms.c_str();
    AttachShader(prog, GL_GEOMETRY_SHADER, geom);
  }
  
  if (fragFileName) {
    shaderfile.open(fragFileName);
    if (shaderfile.is_open()) {
      fragss << shaderfile.rdbuf();
      frags = fragss.str();
      shaderfile.close();
    } else {
      std::cerr << "ERROR: Problem open fragment shader file: " << fragFileName << std::endl;
      exit(1);
    }
    const char *frag = frags.c_str();
    AttachShader(prog, GL_FRAGMENT_SHADER, frag);
  }

  glLinkProgram(prog);
  CheckStatus(prog);
  return prog;
}

