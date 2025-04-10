#include "glfwWindow.hpp"

// stdlib
#include <stdexcept>
#include <iostream>
namespace venus_core{
    
    window_core::window_core( instance_core &instance_coreRef) : m_instance{instance_coreRef}{

        // we do not initialize glfw within the window class so that we can keep the vulkan instance loaded if we ever
        // need to reload/recreate the window. If we cannot create an instance then the window will never open and the
        // processes will terminate with runtime error.

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // the last two parameters are not relevant to vulkan. We do not use them, do not change.
        m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

        createWindowSurface();
    };

    window_core::~window_core(){
        if(m_surface != nullptr){
            vkDestroySurfaceKHR(m_instance.getInstance(), m_surface, nullptr);
        }
        glfwDestroyWindow(m_window);
        std::cout << "window destroyed." << '\n';
    };

    void window_core::createWindowSurface(void){
        if(glfwCreateWindowSurface(m_instance.getInstance(), m_window, nullptr, &m_surface) != VK_SUCCESS){
            throw std::runtime_error("FAILED TO CREATE WINDOW SURFACE!!!");
        }
    }

} // namespace venus_core
