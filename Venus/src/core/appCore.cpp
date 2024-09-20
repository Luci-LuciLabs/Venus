#include "appCore.hpp"
#include "windowCore.hpp"

namespace venusCore{
  appCore::appCore(){};
  appCore::~appCore(){};

  void appCore::run(){
    while(!window_core.shouldClose()){
      glfwPollEvents();
    }
  }
}