#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace venusCore{
  class windowCore{
    public:
      windowCore(int w, int h, std::string name);
      ~windowCore();

      windowCore(const windowCore &) = delete;
      windowCore &operator=(const windowCore &) = delete;

      bool shouldClose() {return glfwWindowShouldClose(window);}

    private:
      int width;
      int height;
      const std::string title;
      GLFWwindow *window;

      void initWindow();
  };
}