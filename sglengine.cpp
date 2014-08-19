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

  return 0;
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
  // const char *imagepath="textures/uvtemplate.bmp";
  GLuint texID;
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

