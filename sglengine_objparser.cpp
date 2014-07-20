#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "sglengine.h"

int SGLEngine::ObjParser(std::string filepath, std::vector<float> &out_vertices, std::vector<float> &out_normals, std::vector<unsigned int> &out_indices) {
  // Simple importer for object files.
  // Settings in Blender: - Triangles
  //                      - Normals
  //                      - No UV !!!
  //
  std::vector<float> tmp_vertices;
  std::vector<float> tmp_normals;
  std::vector<unsigned int> tmp_indices_v, tmp_indices_n;
  std::vector<float> vertices;
  std::vector<float> normals;

  float vx, vy, vz, nx, ny, nz;
  unsigned int iv1, iv2, iv3, in1, in2, in3;

  FILE *file = fopen(filepath.c_str(), "r");
  if( file == NULL ) {
    printf("Could not open file!\n");
    return 1;
  }

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
      // UV map not yet implemented !
    } else if (strcmp(lineheader, "vn") == 0 ) { // normals
      int cnt = fscanf(file, "%f %f %f\n", &nx, &ny, &nz );
      if ( cnt != 3 )
        printf("Something went terrible wrong with reading the normals!!!\n");
      tmp_normals.push_back(nx);
      tmp_normals.push_back(ny);
      tmp_normals.push_back(nz);
    } else if (strcmp(lineheader, "f") == 0 ) { // indices
      int cnt = fscanf(file, "%d//%d %d//%d %d//%d\n", &iv1, &in1, &iv2, &in2, &iv3, &in3 );
      if ( cnt != 6 )
        printf("Something went terrible wrong with reading the indices!!!\n");
      tmp_indices_v.push_back(iv1);
      tmp_indices_v.push_back(iv2);
      tmp_indices_v.push_back(iv3);
      tmp_indices_n.push_back(in1);
      tmp_indices_n.push_back(in2);
      tmp_indices_n.push_back(in3);
    } else if (strcmp(lineheader, "0") == 0 ) { // UV
      // we ignore the object name for now
    }
  }

  #ifdef DEBUG
  for (int i=0; i<tmp_vertices.size(); i+=3) {
    printf("DEBUG: %0.6f %0.6f %0.6f\n", tmp_vertices[i], tmp_vertices[i+1], tmp_vertices[i+2]);
  }

  for (int i=0; i<tmp_indices_n.size(); i+=3) {
    printf("DEBUG: %d//%d %d//%d %d//%d\n", tmp_indices_v[i], tmp_indices_n[i], 
      tmp_indices_v[i+1], tmp_indices_n[i+1], tmp_indices_v[i+2], tmp_indices_n[i+2]);
  }
  #endif

  for(unsigned int i=0; i<tmp_indices_v.size(); i++) {
    unsigned int vindex = tmp_indices_v[i];
    unsigned int nindex = tmp_indices_n[i];

    vertices.push_back( tmp_vertices[ (vindex - 1)*3 ] );
    vertices.push_back( tmp_vertices[ (vindex - 1)*3 + 1 ] );
    vertices.push_back( tmp_vertices[ (vindex - 1)*3 + 2 ] );
    
    normals.push_back( tmp_normals[ (nindex - 1)*3 ] );
    normals.push_back( tmp_normals[ (nindex - 1)*3 + 1 ] );
    normals.push_back( tmp_normals[ (nindex - 1)*3 + 2 ] );
  }
  
  #ifdef DEBUG
  for (int i=0; i<vertices.size(); i+=3) {
    printf("DEBUG: %d : %0.6f %0.6f %0.6f\n", i/3+1, vertices[i], vertices[i+1], vertices[i+2]);
  }
  #endif
  
  // Create an index for the vertices
  out_vertices.resize(0);
  out_normals.resize(0);
  out_indices.resize(0);

  std::vector<float> vertex;
  std::vector<float> normal;
  vertex.resize(3);
  normal.resize(3);

  unsigned int result;

  unsigned int index=0;

  for(unsigned int i=0; i<vertices.size(); i += 3) {
    vertex[0] = vertices.at(i + 0);
    vertex[1] = vertices.at(i + 1);
    vertex[2] = vertices.at(i + 2);
    normal[0] = normals.at(i + 0);
    normal[1] = normals.at(i + 1);
    normal[2] = normals.at(i + 2);

    bool res = ObjFindIndex(out_vertices, out_normals, vertex, normal, result);

    if ( res == true ) {
      out_indices.push_back(result);
    } else {
      out_vertices.push_back(vertex[0]);
      out_vertices.push_back(vertex[1]);
      out_vertices.push_back(vertex[2]);
      out_normals.push_back(normal[0]);
      out_normals.push_back(normal[1]);
      out_normals.push_back(normal[2]);

      out_indices.push_back(index);
      index++;
    }
  }

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

