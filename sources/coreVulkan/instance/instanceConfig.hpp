#ifndef VENUS_VULKAN_INSTANCE_CONFIG_HPP
#define VENUS_VULKAN_INSTANCE_CONFIG_HPP

// stdlib
#include <vector>

namespace venus_core{
    
    #ifdef NDEBUG
        inline static const bool ENABLE_VALIDATION_LAYERS = false;
    #else
        inline static const bool ENABLE_VALIDATION_LAYERS = true;
    #endif

    inline static std::vector<const char *> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

} // namespace venus_core


#endif // VENUS_VULKAN_INSTANCE_CONFIG_HPP