#include "windowCore.hpp"

#include <stdexcept>

namespace venusCore{
  windowCore::windowCore(int w, int h, std::string name) : width(w), height(h), title(name){
    initWindow();
  }

  windowCore::~windowCore(){
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void windowCore::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  }
}