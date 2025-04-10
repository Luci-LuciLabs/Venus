#include "swapchainSupportDetails.hpp"

namespace venus_core{

    SwapchainSupportDetails querySwapchainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface){
        SwapchainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.surfaceCapabilites);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
        if(formatCount != 0){
            details.surfaceFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.surfaceFormats.data());
        }

        uint32_t presentModes = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModes, nullptr);
        if(presentModes != 0){
            details.presentModes.resize(presentModes);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModes, details.presentModes.data());
        }

        return details;

    }
} // namespace venus_core
