#include <iostream>
#include <thread>

#include "sglengine.h"

class MySGLEngine : public SGLEngine {
  private:
    void Render();
};

void MySGLEngine::Render() {

}

int main() {
  MySGLEngine e;

  e.Init();

  std::cout << "Starting main loop..." << std::endl;
  e.Run();
}
