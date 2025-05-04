#include "window.hpp"

// stdlib
#include <cassert>

namespace venus::rendersystem {

	window::window() {}
	window::~window() {}


	bool window::createWindow(void) {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(m_WIDTH, m_HEIGHT, m_TITLE.c_str(), nullptr, nullptr);
		if(m_window == nullptr) {
			return false;
		}
		return true;
	}

  void window::destroyWindow(void) {
    if(m_window != nullptr){
      glfwDestroyWindow(m_window);
    }
      m_window = nullptr;
	}

	bool window::createSurface(VkInstance instance){
    assert(instance != nullptr);
    if(glfwCreateWindowSurface(instance, m_window, nullptr, &m_surface) != VK_SUCCESS){
      return false;
    }

    return true;
  }

  void window::destroySurface(VkInstance instance){
    assert(instance != nullptr);
    if(m_surface != VK_NULL_HANDLE){
      vkDestroySurfaceKHR(instance, m_surface, nullptr);
    }
    m_surface = VK_NULL_HANDLE;
  }

}  // namespace venus::rendersystem
