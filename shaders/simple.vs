#version 330 core

// in vec4 in_position;

void main() {
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_FrontColor = gl_Color; //Added line
  gl_BackColor = gl_Color; //Added line
}
