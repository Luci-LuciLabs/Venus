#include "queueFamilyIndices.hpp"

// stdlib
#include <vector>

namespace venus_core{
    
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface){
        QueueFamilyIndices indices;

        // we get queueFamilyCount so we can size the family vector correctly
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        // index for QueueFamilyIndices, DO NOT CHANGE;
        int index = 0;
        // we loop over each family and update indices so that we know our families are supported.
        for(const auto &queueFamily : queueFamilies){
            if(indices.isComplete()){
                break;
            }

            if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.graphicsFamily = index;
            }

            VkBool32 presentQueueSupported = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &presentQueueSupported);
            if(queueFamily.queueCount > 0 && presentQueueSupported){
                indices.presentFamily = index;
            }

            index++;
        }
        // should ALWAYS return indices, DO NOT CHANGE.
        return indices;
    }

} // namespace venus_core
