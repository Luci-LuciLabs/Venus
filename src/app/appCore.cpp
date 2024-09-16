#include "appCore.hpp"

namespace CORE{
  appCORE::appCORE(){}
  appCORE::~appCORE(){}

  void appCORE::run(){
    while(!window_core.shouldClose()){
      glfwPollEvents();
    }
  }
}