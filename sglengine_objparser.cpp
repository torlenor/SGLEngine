#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "sglengine.h"

int SGLEngine::ObjParser(std::string filepath, SGLEngine::Object &out_object) {
  // Simple importer for object files.
  // Settings in Blender: - Triangles
  //                      - Normals
  //                      - No UV !!!
  //
  std::vector<float> tmp_vertices;
  std::vector<float> tmp_normals;
  std::vector<float> tmp_uvs;
  std::vector<unsigned int> tmp_indices_v, tmp_indices_n, tmp_indices_uv;
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> uvs;

  float vx, vy, vz, nx, ny, nz, uu, vv;
  unsigned int iv1, iv2, iv3, in1, in2, in3, iuv1, iuv2, iuv3;

  FILE *file = fopen(filepath.c_str(), "r");
  if( file == NULL ) {
    printf("Could not open file!\n");
    return 1;
  }

  std::cout << "Loading " << filepath << " ..." << std::endl; 

  bool usesUVs=false;

  while(1) {
    char lineheader[128];
    int res = fscanf(file, "%s", lineheader);
    if (res == EOF)
      break; // END OF FILE

    if (strcmp(lineheader, "v") == 0 ) { // vertices
      int cnt = fscanf(file, "%f %f %f\n", &vx, &vy, &vz );
      if ( cnt != 3 )
        printf("Something went terrible wrong with reading the vertices!!!\n");
      tmp_vertices.push_back(vx);
      tmp_vertices.push_back(vy);
      tmp_vertices.push_back(vz);
    } else if (strcmp(lineheader, "vt") == 0 ) { // UV
      usesUVs=true;

      int cnt = fscanf(file, "%f %f\n", &uu, &vv );
      if ( cnt != 2 )
        printf("Something went terrible wrong with reading the uvs!!!\n");
      tmp_uvs.push_back(uu);
      tmp_uvs.push_back(vv);
    } else if (strcmp(lineheader, "vn") == 0 ) { // normals
      int cnt = fscanf(file, "%f %f %f\n", &nx, &ny, &nz );
      if ( cnt != 3 )
        printf("Something went terrible wrong with reading the normals!!!\n");
      tmp_normals.push_back(nx);
      tmp_normals.push_back(ny);
      tmp_normals.push_back(nz);
    } else if (strcmp(lineheader, "f") == 0 ) { // indices
      if (usesUVs) {
        int cnt = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &iv1, &iuv1, &in1, &iv2, &iuv2, &in2, &iv3, &iuv3, &in3 );
        if ( cnt != 9 )
          printf("Something went terrible wrong with reading the indices (uv version)!!!\n");
        tmp_indices_v.push_back(iv1);
        tmp_indices_v.push_back(iv2);
        tmp_indices_v.push_back(iv3);
        tmp_indices_n.push_back(in1);
        tmp_indices_n.push_back(in2);
        tmp_indices_n.push_back(in3);
        tmp_indices_uv.push_back(iuv1);
        tmp_indices_uv.push_back(iuv2);
        tmp_indices_uv.push_back(iuv3);
      } else {
        int cnt = fscanf(file, "%d//%d %d//%d %d//%d\n", &iv1, &in1, &iv2, &in2, &iv3, &in3 );
        if ( cnt != 6 )
          printf("Something went terrible wrong with reading the indices (non-uv version)!!!\n");
        tmp_indices_v.push_back(iv1);
        tmp_indices_v.push_back(iv2);
        tmp_indices_v.push_back(iv3);
        tmp_indices_n.push_back(in1);
        tmp_indices_n.push_back(in2);
        tmp_indices_n.push_back(in3);
      }
    } else if (strcmp(lineheader, "0") == 0 ) { // UV
      // we ignore the object name for now
    }
  }

  std::cout << "UV = " << usesUVs << std::endl;

  #ifdef DEBUG
  for (int i=0; i<tmp_vertices.size(); i+=3) {
    printf("DEBUG: %0.6f %0.6f %0.6f\n", tmp_vertices[i], tmp_vertices[i+1], tmp_vertices[i+2]);
  }

  for (int i=0; i<tmp_indices_n.size(); i+=3) {
    printf("DEBUG: %d//%d %d//%d %d//%d\n", tmp_indices_v[i], tmp_indices_n[i], 
      tmp_indices_v[i+1], tmp_indices_n[i+1], tmp_indices_v[i+2], tmp_indices_n[i+2]);
  }
  #endif

  unsigned int vindex, nindex, uvindex;
  for(unsigned int i=0; i<tmp_indices_v.size(); i++) {
    vindex = tmp_indices_v[i];
    nindex = tmp_indices_n[i];
    if (usesUVs) 
      uvindex = tmp_indices_uv[i];

    vertices.push_back( tmp_vertices[ (vindex - 1)*3 ] );
    vertices.push_back( tmp_vertices[ (vindex - 1)*3 + 1 ] );
    vertices.push_back( tmp_vertices[ (vindex - 1)*3 + 2 ] );
    
    normals.push_back( tmp_normals[ (nindex - 1)*3 ] );
    normals.push_back( tmp_normals[ (nindex - 1)*3 + 1 ] );
    normals.push_back( tmp_normals[ (nindex - 1)*3 + 2 ] );
   
    if (usesUVs) {
      uvs.push_back( tmp_uvs[ (uvindex - 1)*2 ] );
      uvs.push_back( tmp_uvs[ (uvindex - 1)*2 + 1 ] );
    }
  }
  
  #ifdef DEBUG
  for (int i=0; i<vertices.size(); i+=3) {
    printf("DEBUG: %d : %0.6f %0.6f %0.6f\n", i/3+1, vertices[i], vertices[i+1], vertices[i+2]);
  }
  #endif
  
  // Create an index for the vertices
  out_object.vertices.resize(0);
  out_object.normals.resize(0);
  out_object.uvs.resize(0);
  out_object.indices.resize(0);

  std::vector<float> vertex;
  std::vector<float> normal;
  std::vector<float> uv;
  vertex.resize(3);
  normal.resize(3);
  if (usesUVs)
    uv.resize(2);

  unsigned int result;

  unsigned int index=0;

  for (unsigned int i=0; i<vertices.size(); i += 3) {
    vertex[0] = vertices.at(i + 0);
    vertex[1] = vertices.at(i + 1);
    vertex[2] = vertices.at(i + 2);
    normal[0] = normals.at(i + 0);
    normal[1] = normals.at(i + 1);
    normal[2] = normals.at(i + 2);
    if (usesUVs) {
      uv[0] = uvs.at(i/3*2 + 0);
      uv[1] = uvs.at(i/3*2 + 1);
    }

    bool res;
    if (usesUVs) {
      res = ObjFindIndex(out_object.vertices, out_object.normals, out_object.uvs, vertex, normal, uv, result);
    } else {
      res = ObjFindIndex(out_object.vertices, out_object.normals, vertex, normal, result);
    }

    if (res == true) {
      out_object.indices.push_back(result);
    } else {
      out_object.vertices.push_back(vertex[0]);
      out_object.vertices.push_back(vertex[1]);
      out_object.vertices.push_back(vertex[2]);
      out_object.normals.push_back(normal[0]);
      out_object.normals.push_back(normal[1]);
      out_object.normals.push_back(normal[2]);
      if (usesUVs) {
        out_object.uvs.push_back(uv[0]);
        out_object.uvs.push_back(uv[1]);
      }

      out_object.indices.push_back(index);
      index++;
    }
  }

  // Store the status of UVs
  out_object.usesUVs = usesUVs;

  return 0;
}

inline bool SGLEngine::ObjIsNear(float a, float b) {
  return fabs(a-b) < 0.001f;
}

bool SGLEngine::ObjFindIndex(std::vector<float> &out_vertices, std::vector<float> &out_normals, std::vector<float> &vertex, std::vector<float> &normal, unsigned int &result) {
  for(unsigned int i=0; i<out_vertices.size(); i += 3) {
    if (  ObjIsNear(out_vertices[i], vertex[0]) &&
          ObjIsNear(out_vertices[i+1], vertex[1]) &&
          ObjIsNear(out_vertices[i+2], vertex[2]) &&
          ObjIsNear(out_normals[i], normal[0]) &&
          ObjIsNear(out_normals[i+1], normal[1]) &&
          ObjIsNear(out_normals[i+2], normal[2]) ) {
      result = i;
      return true;
    } else {
      return false;
    }
  }

  return false; // SHOULD NEVER BE CALLED
}

bool SGLEngine::ObjFindIndex(std::vector<float> &out_vertices, std::vector<float> &out_normals, std::vector<float> &out_uvs, std::vector<float> &vertex, std::vector<float> &normal, std::vector<float> &uv, unsigned int &result) {
  for(unsigned int i=0; i<out_vertices.size(); i += 3) {
    if (  ObjIsNear(out_vertices[i], vertex[0]) &&
          ObjIsNear(out_vertices[i+1], vertex[1]) &&
          ObjIsNear(out_vertices[i+2], vertex[2]) &&
          ObjIsNear(out_normals[i], normal[0]) &&
          ObjIsNear(out_normals[i+1], normal[1]) &&
          ObjIsNear(out_normals[i+2], normal[2]) &&
          ObjIsNear(out_uvs[i/3*2], uv[0]) &&
          ObjIsNear(out_uvs[i/3*2+1], uv[1])
          ) {
      result = i;
      return true;
    } else {
      return false;
    }
  }

  return false; // SHOULD NEVER BE CALLED
}

