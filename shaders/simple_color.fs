#version 140

in vec2 UV;

out vec3 fColor;

in vec3 fragmentColor;
in vec3 fragmentNormal;

uniform sampler2D myTextureSampler;

void main() {
  vec3 MaterialAmbientColor = vec3(0.5,0.5,0.5) * fragmentColor;

  vec3 lightvec = normalize(vec3(1.0, 1.0, 1.0));
  float cosTheta = clamp(dot(fragmentNormal, lightvec), 0, 1);
  vec3 LightColor = vec3(1.0, 1.0, 1.0);

  // fColor = MaterialAmbientColor + (fragmentColor)*LightColor*cosTheta;
  fColor = MaterialAmbientColor + (fragmentColor)*cosTheta;
}
