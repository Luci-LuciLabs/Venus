#ifndef VENUS_GLFW_WINDOW_HPP
#define VENUS_GLFW_WINDOW_HPP

#include <GLFW/glfw3.h>

// stdlib
#include <string>

namespace venus_core{
    class window_core{
        public:
            window_core();
            ~window_core();

            window_core(const window_core &) = delete;
            window_core &operator=(const window_core &) = delete;

            bool shouldClose(void) { return glfwWindowShouldClose(member_window); }

        private:

            GLFWwindow* member_window;
            uint16_t width = 800;
            uint16_t height = 600;
            std::string name = "venus";
    };
} // namespace venus_core


#endif // VENUS_GLFW_WINDOW_HPP