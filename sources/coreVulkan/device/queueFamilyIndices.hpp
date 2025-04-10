#ifndef VENUS_DEVICE_QUEUE_FAMILY_INDICES_HPP
#define VENUS_DEVICE_QUEUE_FAMILY_INDICES_HPP

#include "../volk/volk.h"

// stdlib
#include <optional>

namespace venus_core{
    
    // if any changes are made to QueueFamilyIndices then you must also update findQueueFamilies()

    struct QueueFamilyIndices{

        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete(void){
            return graphicsFamily.has_value() && presentFamily.has_value();
        }

    }; // QueueFamilyIndices

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

} // namespace venus_core


#endif // VENUS_DEVICE_QUEUE_FAMILY_INDICES_HPP