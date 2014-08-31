#ifndef LOADBMP_H
#define LOADBMP_H

#include <GL/glew.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>

GLuint loadBMP_custom(const char * imagepath);
	
#endif // LOADBMP_H
