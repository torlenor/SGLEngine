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

#include "measuretime.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

void getGLError(std::string filename, int line) {
  GLenum glerr;
  while ((glerr = glGetError()) != GL_NO_ERROR) {
    std::cerr << "WARNING: In " << filename << " at line " << line << "\n" 
      << "OpenGL error: " << glerr << std::endl;
  }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}   

static void error_callback(int error, const char* description) {
  fputs(description, stderr);
}



SGLEngine::SGLEngine() {

}

SGLEngine::~SGLEngine() {

}

int SGLEngine::Init() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);
  

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_SAMPLES,4);

  window = glfwCreateWindow(1024, 800, "Simple example", NULL, NULL);
  if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  getGLError(__FILE__, __LINE__);
  
  glfwSetKeyCallback(window, key_callback);
  getGLError(__FILE__, __LINE__);

  glewExperimental = GL_TRUE;
  if( GLEW_OK != glewInit() )
    exit(EXIT_FAILURE);
  getGLError(__FILE__, __LINE__);

  fputs("Window Created\n", stdout);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  getGLError(__FILE__, __LINE__);
  
  glEnable(GL_CULL_FACE);

  return 0;
}

void SGLEngine::InfoObject(SGLEngine::Object &obj) {
  std::cout << "Object Info:\n"
    << "\tNumber of vertices = " << obj.vertices.size()/3 << "\n" 
    << "\tNumber of colors = " << obj.colors.size()/3 << "\n" 
    << "\tNumber of normals = " << obj.normals.size()/3 << "\n" 
    << "\tNumber of UVs = " << obj.uvs.size()/2 << "\n" 
    << "\tNumber of indices = " << obj.indices.size() << "\n" 
    << "\tVAOid = " << obj.vaoid[0] << "\n" 
    << "\tShader number = " << obj.shader << "\n" 
    << "\tCurrent position = (" << obj.currentPos.x << "," << obj.currentPos.y << "," << obj.currentPos.z << ") \n" 
    << "\tCurrent velocity = (" << obj.currentVel.x << "," << obj.currentVel.y << "," << obj.currentVel.z << ") \n" 
    << "\tBounding Box:\n"
    << "\t\t +x= " << obj.boundingBox.at(0) << "\n"
    << "\t\t -x= " << obj.boundingBox.at(1) << "\n"
    << "\t\t +y= " << obj.boundingBox.at(2) << "\n"
    << "\t\t -y= " << obj.boundingBox.at(3) << "\n"
    << "\t\t +z= " << obj.boundingBox.at(4) << "\n"
    << "\t\t -z= " << obj.boundingBox.at(5) << "\n"
    << "END Object info" << std::endl;
}

int SGLEngine::SetupObject(SGLEngine::Object &obj) {
  getGLError(__FILE__, __LINE__);

  std::cout << "Generating VAOs..." << std::endl;
  obj.vaoid.resize(1);

  glGenVertexArrays(1, &obj.vaoid[0]);
  glBindVertexArray(obj.vaoid[0]);
  getGLError(__FILE__, __LINE__);
  
  std::cout << "Copy buffers..." << std::endl;
  // Vertices //
  obj.bufferid.resize(1);
  glGenBuffers(1, &obj.bufferid[0]);
  glBindBuffer(GL_ARRAY_BUFFER, obj.bufferid[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*obj.vertices.size(), &obj.vertices[0], GL_STATIC_DRAW);
  getGLError(__FILE__, __LINE__);

  glEnableVertexAttribArray(0);
  getGLError(__FILE__, __LINE__);
  glVertexAttribPointer(
      0,        // attribute. No particular reason for 1, but must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void*)0  // array buffer offset
  );
  getGLError(__FILE__, __LINE__);
  // Vertices END //

  // Indices //
  if (obj.isIndexed) {
    glGenBuffers(1, &obj.indid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.indid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indices.size() * sizeof(GLuint),
        &obj.indices[0], GL_STATIC_DRAW);
  }
  getGLError(__FILE__, __LINE__);
  // INDICES END //
  
  // COLOR //
  if (obj.colors.size() > 0) {
    glGenBuffers(1, &obj.colorid);
    glBindBuffer(GL_ARRAY_BUFFER, obj.colorid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*obj.colors.size(), &obj.colors[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, obj.colorid);
    glVertexAttribPointer(
        1,        // attribute. No particular reason for 1, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void*)0  // array buffer offset
    );
    getGLError(__FILE__, __LINE__);
  }
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
  getGLError(__FILE__, __LINE__);
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
  getGLError(__FILE__, __LINE__);
  // UVS END //

  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

  getGLError(__FILE__, __LINE__);

  // Build a bounding box
  float maxx=-10000, minx=10000;
  float maxy=-10000, miny=10000;
  float maxz=-10000, minz=10000;
  float vertexx=0, vertexy=0, vertexz=0;
  for (unsigned int v=0; v<obj.vertices.size(); v += 3) {
    vertexx = obj.vertices.at(v+0);
    vertexy = obj.vertices.at(v+1);
    vertexz = obj.vertices.at(v+2);
    // x direction
    if (vertexx > maxx ) maxx = vertexx;
    if (vertexx < minx ) minx = vertexx;

    // y direction
    if (vertexy > maxy ) maxy = vertexy;
    if (vertexy < miny ) miny = vertexy;
    
    // z direction
    if (vertexz > maxz ) maxz = vertexz;
    if (vertexz < minz ) minz = vertexz;
  }

  obj.boundingBox.at(0) = maxx;
  obj.boundingBox.at(1) = minx;
  obj.boundingBox.at(2) = maxy;
  obj.boundingBox.at(3) = miny;
  obj.boundingBox.at(4) = maxz;
  obj.boundingBox.at(5) = minz;

  return 0;
}

int SGLEngine::UpdateObject(SGLEngine::Object &obj) {
  std::cout << "Updating object..." << std::endl;

  glBindBuffer(GL_ARRAY_BUFFER, obj.bufferid[0]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*obj.vertices.size(), &obj.vertices[0]);
  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  
  glBindBuffer(GL_ARRAY_BUFFER, obj.colorid);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*obj.colors.size(), &obj.colors[0]);
  glBindBuffer(GL_ARRAY_BUFFER, 0); 

  return 0;
}

int SGLEngine::DeleteObject(SGLEngine::Object &obj) {
    if (obj.textureid != 0) {
      glDeleteTextures(1, &obj.textureid);
      obj.textureid = 0;
    }

    if (obj.bufferid[0] != 0) {
        glDeleteBuffers(obj.bufferid.size(), &obj.bufferid[0]);
        obj.bufferid[0] = 0;
    }
    
    if (obj.colorid != 0) {
        glDeleteBuffers(1, &obj.colorid);
        obj.colorid = 0;
    }
       
    if (obj.vaoid[0] != 0) {
        glDeleteVertexArrays(obj.vaoid.size(), &obj.vaoid[0]);
        obj.vaoid[0] = 0;
    } 

    return 0;
}

int SGLEngine::RemoveObject(SGLEngine::Scene &scene, unsigned int objid) {
  SGLEngine::Object obj = scene.objects.at(objid);
  scene.objects.erase(scene.objects.begin() + objid);
  DeleteObject(obj);

  return 0;
}

int SGLEngine::SetupScene() {
  // has to be implemented in derived class
  
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

void SGLEngine::UpdateCamera(SGLEngine::Scene &scene, const double deltaTime) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    static double xorigin=xpos;

    scene.camRotY += scene.deltaCamRotY*(float)(deltaTime) - (xpos-xorigin)*(float)deltaTime*0.1;
    glfwSetCursorPos(window, 600, 400);
    xorigin=600;

    static double yorigin=ypos;

    scene.camRotZ += scene.deltaCamRotZ*(float)(deltaTime) - (ypos-yorigin)*(float)deltaTime*0.1;
    glfwSetCursorPos(window, 600, 400);
    yorigin=400;

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

    glClearColor(0.0f, 0.2f, 0.2f , 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    ratio = width/(float)height;
    ratio = ratio;

    UpdateCamera(scene, deltaTime);

    // FOVY is in radiant
    glm::mat4 Projection = glm::perspective((float)(70.0f/360.0f*2.0f*M_PI), ratio, 0.1f, 1000.0f);

    glm::mat4 View = glm::lookAt(
      scene.camPosition, // Camera in World Space
      scene.camPosition + glm::vec3(sin(scene.camRotY), sin(scene.camRotZ), cos(scene.camRotY)), // and look at
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    ); 

    Collision(scene);
      
    for(auto &obj : scene.objects) {
      obj.DoPhys((float)deltaTime);
      obj.currentPos += obj.currentVel*(float)deltaTime;

      float yaw = (float)(obj.currentRot.x/360.0f*2.0f*M_PI);
      float pitch = (float)(obj.currentRot.y/360.0f*2.0f*M_PI);
      float roll = (float)(obj.currentRot.z/360.0f*2.0f*M_PI);
      glm::mat4 eulerRot = glm::eulerAngleYXZ(yaw, pitch, roll);
      glm::mat4 Model = glm::translate(glm::mat4(1.0f), obj.currentPos);
      Model = glm::scale(Model, obj.scale);

      glm::mat4 MVP = Projection * View * Model * eulerRot;

      glBindVertexArray(obj.vaoid[0]);
      glUseProgram(obj.shader);
    
      GLuint MatrixID = glGetUniformLocation(obj.shader, "MVP");
      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

      if (obj.usesUVs) {
        GLuint TextureID  = glGetUniformLocation(obj.shader, "myTextureSampler");
        glUniform1i(TextureID, 0); 
      }

      if (obj.isIndexed) {
        glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT, 0); 
      } else {
        glDrawArrays(GL_TRIANGLES, 0, obj.vertices.size()/3); 
      }   

      GLenum glerr;
      while ((glerr = glGetError()) != GL_NO_ERROR)
        std::cerr << "OpenGL error: " << glerr << std::endl;
    }
  
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

bool SGLEngine::CheckCollision(SGLEngine::Object &obj1, SGLEngine::Object &obj2) {
  // returns true if collision between obj1 and obj2
  if ( obj2.currentPos.x + obj2.boundingBox[1]*obj2.scale.x < (obj1.currentPos.x + obj1.boundingBox[0]*obj1.scale.x)
      && obj2.currentPos.x + obj2.boundingBox[0]*obj2.scale.x > obj1.currentPos.x + obj1.boundingBox[1]*obj1.scale.x
      && obj2.currentPos.y + obj2.boundingBox[3]*obj2.scale.y < obj1.currentPos.y + obj1.boundingBox[2]*obj1.scale.y
      && obj2.currentPos.y + obj2.boundingBox[2]*obj2.scale.y > obj1.currentPos.y + obj1.boundingBox[3]*obj1.scale.y
      && obj2.currentPos.z + obj2.boundingBox[5]*obj2.scale.z < obj1.currentPos.z + obj1.boundingBox[4]*obj1.scale.z
      && obj2.currentPos.z + obj2.boundingBox[4]*obj2.scale.z > obj1.currentPos.z + obj1.boundingBox[5]*obj1.scale.z
   ) {
    return true;
  } else {
    return false;
  }
}


void CalcNewVel(const glm::vec3 &v1, const glm::vec3 &v2, float m1, float m2, glm::vec3 &newv1, glm::vec3 &newv2) {
  float k = 0.2;
  float tmass = m1+m2;
  
  newv1.x = (m1*v1.x + m2*v2.x - k*m2*(v1.x - v2.x)) / tmass;
  newv1.y = (m1*v1.y + m2*v2.y - k*m2*(v1.y - v2.y)) / tmass;
  newv1.z = (m1*v1.z + m2*v2.z - k*m2*(v1.z - v2.z)) / tmass;

  newv2.x = (m1*v1.x + m2*v2.x - k*m1*(v2.x - v1.x)) / tmass;
  newv2.y = (m1*v1.y + m2*v2.y - k*m1*(v2.y - v1.y)) / tmass;
  newv2.z = (m1*v1.z + m2*v2.z - k*m1*(v2.z - v1.z)) / tmass;
}

void SGLEngine::Collision(SGLEngine::Scene &scene1) {
  for (int j=0; j<(int)scene1.objects.size(); j++) {
    if (scene1.objects[j].doColl) {
    SGLEngine::Object &obj1 = scene1.objects[j];
      for (int i=0; i<(int)scene1.objects.size(); i++) {
        if ( i != j ) {
          SGLEngine::Object &obj2 = scene1.objects[i];
          if (CheckCollision(obj1, obj2)) {
            obj1.physGrav=true;
            obj2.physGrav=true;

            CalcNewVel(obj1.currentVel, obj2.currentVel,  obj1.gravMass, obj2.gravMass, obj1.currentVel, obj2.currentVel);

            obj2.currentRot += glm::vec3((rand()/(float)RAND_MAX-0.5)*15.0f, (rand()/(float)RAND_MAX-0.5)*15.0f, (rand()/(float)RAND_MAX-0.5)*15.0f);
          }
        }
      }
    }
  }

  for (unsigned int i=0; i<scene1.objects.size(); i++) {
    if (scene1.objects[i].currentPos.y < 0.0f && i != scene1.objects.size() ) {
      scene1.objects[i].physGrav=false;
      scene1.objects[i].doColl = false;
      scene1.objects[i].currentVel = glm::vec3(0.0f, 0.0f, 0.0f);
      // scene1.objects[i].currentPos.y = 0.0f;
    }
  }
}

void SGLEngine::Physics(SGLEngine::Scene &scene) {
  struct timespec tim1;
  tim1.tv_sec=0;
  tim1.tv_nsec=100*(1000000);
  while (!glfwWindowShouldClose(window)) {
    Collision(scene);
    nanosleep(&tim1, NULL);
  }
}
