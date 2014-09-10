#include "sglengine.h"

#include <iostream>

void SGLEngine::Physics::Gravity(float dt) {
  this->currentVel += dt*gravG*glm::vec3(0.0,1.0,0.0);
}
