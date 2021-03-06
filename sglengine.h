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

#include <GL/glew.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif

#include <GLFW/glfw3.h>

#include <list>
#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SGLEngine {
  public:
    class ObjectData {
      public:
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> normals;
        std::vector<GLfloat> colors;
        std::vector<GLfloat> uvs;
        std::vector<GLuint> indices;
        
        bool isIndexed;
        bool usesUVs;
    };
    
    class Physics {
      public:
        Physics() {
          boundingBox.resize(6);
        }

        bool physGrav;
        float gravG;
        float gravMass;

        bool doColl;
        bool collDetect;
        std::vector<float> boundingBox; // +x,-x,+y,-y,+z,-z direction

        void DoPhys(float dt) {
          if (physGrav) Gravity(dt);
        };
        
        glm::vec3 currentVel; // Current velocity in units/s

      private:
        void Gravity(float dt); // this will change currentVel;
    };

    class Object : public Physics, public ObjectData {
      public:
        std::vector<GLuint> vaoid;
        std::vector<GLuint> bufferid;
        GLuint indid;
        GLuint shader;
        GLuint textureid;
        GLuint colorid;

        glm::vec3 currentPos; // Current position
        glm::vec3 currentRot; // Rotation in (yaw, pitch, roll)

        glm::vec3 scale;
    };
    
    struct Scene {
      std::vector<Object> objects;
      std::list<Object> objectsToRender;

      glm::vec3 camPosition;
      glm::vec3 camPositionOffset;
      glm::vec3 deltaCamPosition;

      GLfloat camRotY;
      GLfloat deltaCamRotY;
      
      GLfloat camRotZ;
      GLfloat deltaCamRotZ;
    };

    SGLEngine();
    ~SGLEngine();

    int Init(); // Inititializes OpenGL
    
    void InfoObject(Object &obj); // Print infos about the Object obj

    int SetupObject(Object &obj); // Create the VAO/VBO for an SGLEngine::Object
    int UpdateObject(Object &obj); // Create the VAO/VBO for an SGLEngine::Object
    int DeleteObject(Object &obj); // Deletes VAO/VBO for an SGLEngine::Object
    int RemoveObject(Scene &scene, unsigned int objid); // Removes object from scene 
    int SetupScene(); // Build the scene prior to rendering

    int LoadScene(SGLEngine::Scene &scene, std::string filename);

    void RenderScene(Scene &scene); // Render the whole scene

    void CheckStatus(GLuint obj);
    void AttachShader(GLuint program, GLenum type, const char* src);
    GLuint LoadShaders(const char* vertFileName, const char* geomFileName, const char* fragFileName);

    void Run(); // Main loop

    int ObjParser(std::string filepath, Object &out_object);
        
    void PrintFPS();

    void Physics(Scene &scene);
    void Collision(Scene &scene1);
    bool CheckCollision(Object &obj1, Object &obj2);
    
  protected:
    GLFWwindow *window;

  private:
    virtual void Render(); // Has to be overloaded to be useful

    void UpdateCamera(Scene &csene, const double deltaTime);

    inline bool ObjIsNear(float a, float b);
    bool ObjFindIndex(std::vector<float> &out_vertices, 
        std::vector<float> &out_normals, std::vector<float> &vertex, 
        std::vector<float> &normal, unsigned int &result);

    bool ObjFindIndex(std::vector<float> &out_vertices, 
        std::vector<float> &out_normals, std::vector<float> &out_uvs, 
        std::vector<float> &vertex, std::vector<float> &normal, 
        std::vector<float> &uv, unsigned int &result);
};

#endif // SGLENGINE_H
