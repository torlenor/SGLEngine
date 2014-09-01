#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "sglengine.h"

#include "loadbmp.h"

int SGLEngine::LoadScene(SGLEngine::Scene &scene, std::string filename) {
  FILE *pFile=0;
  pFile = fopen(filename.c_str(), "r");
  if (pFile == NULL) {
    printf("Could not open scene file: %s\n", filename.c_str());
    return 1;
  }

  std::cout << "Loading scene..." << std::endl;

  scene.objects.resize(0);

  float xPos,yPos,zPos;
  float xScale, yScale, zScale;
  float rColor, gColor, bColor;
  float yaw, pitch, roll;
  char objfile[256], texturefile[256], vsfile[256], fsfile[256];

  SGLEngine::Object obj1;
  std::vector<SGLEngine::Object> objList;
  std::vector<GLuint> shaderList, textureList;

  while(true) {
    char lineheader[128];
    int res = fscanf(pFile, "%s", lineheader);
    if (res == EOF)
      break;
    if( strcmp(lineheader, "o") == 0 ) { // object
      int cnt = fscanf(pFile, "%s %f %f %f\n", objfile, 
                          &rColor, &gColor, &bColor);
      if( cnt != 4) {
        std::cout << "ERROR: Reading scene file! (object part)" << std::endl;
        return 1;
      }
      SGLEngine::ObjParser(std::string(objfile), obj1);
      obj1.colors.resize( obj1.vertices.size() );
      for(int i=0; i<(int)obj1.colors.size()/3; i++) {
        obj1.colors.at(3*i + 0) = rColor;
        obj1.colors.at(3*i + 1) = gColor;
        obj1.colors.at(3*i + 2) = bColor;
      }
      SGLEngine::SetupObject(obj1);
      InfoObject(obj1);
      objList.push_back(obj1);
    }

    if( strcmp(lineheader, "s" ) == 0 ) { // shader
      int cnt = fscanf(pFile, "%s %s\n", vsfile, fsfile);
      if( cnt != 2) {
        std::cout << "ERROR: Reading scene file! (shader part)" << std::endl;
        return 1;
      }
      std::cout << vsfile << " " << fsfile << std::endl;
      GLuint shader = SGLEngine::LoadShaders( vsfile, NULL, fsfile );
      shaderList.push_back(shader);
    }
    
    if( strcmp(lineheader, "t" ) == 0 ) { // texture
      int cnt = fscanf(pFile, "%s\n", texturefile);
      if( cnt != 1) {
        std::cout << "ERROR: Reading scene file! (texture part)" << std::endl;
        return 1;
      }
      GLuint textureid = loadBMP_custom(texturefile);
      textureList.push_back(textureid);
    }

    if( strcmp(lineheader, "sc" ) == 0 ) { // scene object
      int objnr=0, texturenr=0, shadernr=0;
      int cnt = fscanf(pFile, "%i %i %i %f %f %f %f %f %f %f %f %f\n",
                          &objnr, &texturenr, &shadernr, 
                          &xPos, &yPos, &zPos, 
                          &xScale, &yScale, &zScale, 
                          &yaw, &pitch, &roll);
      if( cnt != 12) {
        std::cout << "ERROR: Reading scene file! (scene description part)" << std::endl;
        return 1;
      }
      obj1 = objList.at(objnr);
      obj1.textureid = textureList.at(texturenr);
      obj1.shader = shaderList.at(shadernr);

      obj1.currentPos = glm::vec3(xPos, yPos, zPos);
      obj1.currentRot = glm::vec3(yaw, pitch, roll);
      obj1.scale = glm::vec3(xScale, yScale, zScale);

      scene.objects.push_back(obj1);
    }
  }

  return 0;
}
