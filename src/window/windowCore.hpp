#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace CORE{
  class windowCORE{
    public:
      windowCORE(int w, int h, std::string name);
      ~windowCORE();

      windowCORE(const windowCORE &) = delete;
      windowCORE &operator=(const windowCORE) = delete;

      void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
      bool shouldClose() {return glfwWindowShouldClose(window);}

    private:
      int width;
      int height;
      const std::string title;
      GLFWwindow *window;

      void initWindow();
  };
}