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

#include <GL/gl.h>
#include <GLFW/glfw3.h>

SGLEngine::SGLEngine() {

}

SGLEngine::~SGLEngine() {

}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

int SGLEngine::Init() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
  }

  fputs("Window Created\n", stdout);

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

GLuint SGLEngine::LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

  return 0;
}

