#include "glfwWindow.hpp"

namespace venus_core{
    
    window_core::window_core(){
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        member_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    };

    window_core::~window_core(){
        glfwTerminate();
    };


} // namespace venus_core
