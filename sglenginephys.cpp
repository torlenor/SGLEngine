#include "sglengine.h"

#include <iostream>

void SGLEngine::Physics::Gravity(float dt) {
  this->currentVel += dt*gravG*glm::vec3(0.0,1.0,0.0);
}

void CheckObjCollision(SGLEngine::Object &obj1, SGLEngine::Object &obj2) {

}

void SGLEngine::CheckCollision( SGLEngine::Scene &scene ) {
  for (unsigned int i=0; i<scene.objects.size(); i++) {
    for (unsigned int j=i+1; j<scene.objects.size(); j++) {
      CheckObjCollision(scene.objects[i], scene.objects[j]);
    }
  }
}
