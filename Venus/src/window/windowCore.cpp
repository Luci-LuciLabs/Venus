#include "windowCore.hpp"

#include <stdexcept>

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

/*
  param 1 = width resolution of window
  param 2 = height resolution of window
  param 3 = name of window
  param 4 = monitor for window display
  param 5 = vulkan does not use
*/
  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}