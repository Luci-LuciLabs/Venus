#ifndef VENUS_WINDOW_HPP
#define VENUS_WINDOW_HPP

#include "appDetails.hpp"

// 3RD PARTY
#include "volk.h"
#include <GLFW/glfw3.h>

namespace venus{

  class Instance;

  class Window{
    public:
      Window(WindowDetails &detailsRef, Instance &instanceRef);
      ~Window();

      Window(const Window&) = delete;
      Window &operator=(const Window&) = delete;

      bool shouldClose(void) const {return glfwWindowShouldClose(m_window);}

      VkSurfaceKHR getSurface(void) const {return m_surface;}

    private:
      WindowDetails &m_details;
      Instance &m_instance;

      GLFWwindow* m_window = nullptr;
      VkSurfaceKHR m_surface = VK_NULL_HANDLE;

      void createSurface();

  };

} // namespace venus

#endif // VENUS_WINDOW_HPP