/*
 * sglengine.h - sglengine class header
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

#ifndef SGLENGINE_H
#define SGLENGINE_H

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>


class SGLEngine {
  public:
    struct Object {
      std::vector<GLfloat> vertices;
      std::vector<GLfloat> normals;
      std::vector<GLfloat> colors;
      std::vector<GLuint> indices;

      GLuint vboid;
      GLuint indid;
      GLuint shader;
    };

    SGLEngine();
    ~SGLEngine();

    int Init(); // Inititializes OpenGL
    void Run(); // Main loop

    int ObjParser(std::string filepath, std::vector<float> &out_vertices, 
        std::vector<float> &out_normals, std::vector<unsigned int> &out_indices);
    
    int ObjParser(std::string filepath, Object &out_object);
    
    GLuint LoadShaders(const char * vertex_file_path, 
        const char * fragment_file_path);

    
    struct Scene {
      std::vector<Object> objects;
    };

  private:
    GLFWwindow *window;
    void Render(); // Has to be overloaded to be useful

    inline bool ObjIsNear(float a, float b);
    bool ObjFindIndex(std::vector<float> &out_vertices, 
        std::vector<float> &out_normals, std::vector<float> &vertex, 
        std::vector<float> &normal, unsigned int &result);
};

#endif // SGLENGINE_H
