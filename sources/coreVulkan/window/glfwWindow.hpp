#ifndef VENUS_GLFW_WINDOW_HPP
#define VENUS_GLFW_WINDOW_HPP

#include "../instance/instance.hpp"

#define VK_NO_PROTOTYPES
#include "../volk/volk.h"
#include <GLFW/glfw3.h>

// stdlib
#include <string>

namespace venus_core{
    class window_core{
        public:
            window_core(instance_core &instance_coreRef);
            ~window_core();

            // window_core should not be copied.
            window_core(const window_core &) = delete;
            window_core &operator=(const window_core &) = delete;

            // window getters
            bool shouldClose(void) { return glfwWindowShouldClose(m_window); }
            VkSurfaceKHR getSurface(void) const {return m_surface;}

        private:

            // innate window members
            GLFWwindow* m_window;
            instance_core &m_instance;
            VkSurfaceKHR m_surface;

            // window config members
            uint16_t width = 800;
            uint16_t height = 600;
            std::string name = "venus";

            void createWindowSurface(void);
    };
} // namespace venus_core


#endif // VENUS_GLFW_WINDOW_HPP