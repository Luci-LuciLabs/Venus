#include "window.hpp"
#include "instance.hpp"
#include "VN_logger.hpp"

#include <stdexcept>

namespace venus{
  
  Window::Window(WindowDetails &detailsRef, Instance &instanceRef) : m_details(detailsRef), m_instance(instanceRef) {

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(m_details.width, m_details.height, m_details.title, nullptr, nullptr);
    if(!m_window){
      VN_LOG_CRITICAL("Failed to create glfw-window.");
      throw std::runtime_error("Failed to create glfw-Window.");
    }
    VN_LOG_INFO("Window-Construction was successful.");

    createSurface();
  }

  Window::~Window(){

    if(m_surface != VK_NULL_HANDLE){
      vkDestroySurfaceKHR(m_instance.getInstance(), m_surface, nullptr);
    }
    m_surface = VK_NULL_HANDLE;

    glfwDestroyWindow(m_window);
    glfwTerminate();

    VN_LOG_INFO("Window-Destruction was successful.");
  }


  void Window::createSurface(){

    if(glfwCreateWindowSurface(m_instance.getInstance(), m_window, nullptr, &m_surface) != VK_SUCCESS){
      VN_LOG_CRITICAL("Failed to create window surface.");  
      throw std::runtime_error("Failed to create window surface.");
    }

    VN_LOG_INFO("Window Surface was created successfully.");
  }

} // namespace venus
