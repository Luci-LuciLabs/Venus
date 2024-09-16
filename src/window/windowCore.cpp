#include "windowCore.hpp"

#include <stdexcept>

namespace CORE{
  windowCORE::windowCORE(int w, int h, std::string name) : width(w), height(h), title(name){
    initWindow();
  }

  windowCORE::~windowCORE(){
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void windowCORE::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  }

}