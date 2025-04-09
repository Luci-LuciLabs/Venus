#ifndef VENUS_GLFW_INSTANCE_EXTENSIONS_HPP
#define VENUS_GLFW_INSTANCE_EXTENSIONS_HPP

// stdlib
#include <vector>

namespace venus_core{
    namespace instanceExtensions{
        
        std::vector<const char*> requiredInstanceExtensions(void);

    } // namespace instanceExtensions   
} // namespace venus_core

#endif // VENUS_GLFW_INSTANCE_EXTENSIONS_HPP