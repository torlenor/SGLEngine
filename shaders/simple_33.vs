#version 330 core

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec3 vertexColor;
layout( location = 2 ) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUV;

out vec3 fragmentColor;
out vec3 fragmentNormal;
out vec2 UV;

uniform mat4 MVP;

void main() {
  // gl_Position = vPosition;
  fragmentColor = vertexColor;
  fragmentNormal = vertexNormal;
  gl_Position = MVP * vPosition;
  UV = vertexUV;
}
