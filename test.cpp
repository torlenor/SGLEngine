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




struct Program
{
    static GLuint Load( const char* vert, const char* geom, const char* frag )
    {
        GLuint prog = glCreateProgram();
        if( vert ) AttachShader( prog, GL_VERTEX_SHADER, vert );
        if( geom ) AttachShader( prog, GL_GEOMETRY_SHADER, geom );
        if( frag ) AttachShader( prog, GL_FRAGMENT_SHADER, frag );
        glLinkProgram( prog );
        CheckStatus( prog );
        return prog;
    }

private:
    static void CheckStatus( GLuint obj )
    {
        GLint status = GL_FALSE, len = 10;
        if( glIsShader(obj) )   glGetShaderiv( obj, GL_COMPILE_STATUS, &status );
        if( glIsProgram(obj) )  glGetProgramiv( obj, GL_LINK_STATUS, &status );
        if( status == GL_TRUE ) return;
        if( glIsShader(obj) )   glGetShaderiv( obj, GL_INFO_LOG_LENGTH, &len );
        if( glIsProgram(obj) )  glGetProgramiv( obj, GL_INFO_LOG_LENGTH, &len );
        std::vector< char > log( len, 'X' );
        if( glIsShader(obj) )   glGetShaderInfoLog( obj, len, NULL, &log[0] );
        if( glIsProgram(obj) )  glGetProgramInfoLog( obj, len, NULL, &log[0] );
        std::cerr << &log[0] << std::endl;
        exit( -1 );
    }

    static void AttachShader( GLuint program, GLenum type, const char* src )
    {
        GLuint shader = glCreateShader( type );
        glShaderSource( shader, 1, &src, NULL );
        glCompileShader( shader );
        CheckStatus( shader );
        glAttachShader( program, shader );
        glDeleteShader( shader );
    }
};
#define GLSL(version, shader) "#version " #version "\n" #shader

const char* vert = GLSL
(
    330 core,
    layout( location = 0 ) in vec4 vPosition;
    void main()
    {
        gl_Position = vPosition;
    }
);

const char* frag = GLSL
(
    330 core,
    out vec4 fColor;
    void main()
    {
        fColor = vec4( 1.0, 0.0, 1.0, 1.0 );
    }
);




class MySGLEngine : public SGLEngine {
  public:
    int SetupScene();
    void Run();
  private:
    GLuint VAOs[1];
    GLuint Buffers[1];
    GLuint programID;

    void Render();
};

int MySGLEngine::SetupScene() {
  //std::string filepath("monkey.obj");
  //if(ObjParser(filepath, obj1) != 0)
  //  std::cout << "Error loading object " << filepath << " !" << std::endl;

  glGetError();
  glfwMakeContextCurrent(window);
  
  GLfloat vertices[3][2] = { 
      { -0.90, -0.90 }, // Triangle 1
      { 0.85, -0.90 },
      { -0.90, 0.85 },
    };  
  
  std::cout << "Generating VAOs..." << std::endl;
  glGenVertexArrays(1, VAOs);
  glBindVertexArray(VAOs[0]);
  
  std::cout << "Copy buffers..." << std::endl;
  glGenBuffers(1, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*2, vertices, GL_STATIC_DRAW);
  
  std::cout << "Loading shaders..." << std::endl;
  GLuint program = Program::Load( vert, NULL, frag );
  glUseProgram(program);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);  

  std::cout << frag << std::endl;

  return 0;
}

void MySGLEngine::Render() {
  std::cout << "Rendering..." << std::endl;
  
  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window))
  {   
      glClear(GL_COLOR_BUFFER_BIT);
  
      glBindVertexArray(VAOs[0]);
      
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      GLenum glerr;
      while ((glerr = glGetError()) != GL_NO_ERROR)
        std::cerr << "OpenGL error: " << glerr << std::endl;

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
