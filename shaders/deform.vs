#version 140
#extension GL_ARB_explicit_attrib_location : require

layout( location = 0 ) in vec4 vPosition;

uniform mat4 MVP;

void main() {
  vec4 v1 = vec4(vPosition);
  v1.z = sin(v1.y)*1.25;
  gl_Position = MVP * v1;
}
