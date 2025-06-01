#ifndef VENUS_PHYSICAL_DEVICE_HPP
#define VENUS_PHYSICAL_DEVICE_HPP

#include "volk.h"
#include "gpuStructs.hpp"



namespace venus{
  
  class PhysicalDevice{
    public:
      PhysicalDevice(const VkInstance& instanceRef, const VkSurfaceKHR& surfaceRef);
      ~PhysicalDevice();

      PhysicalDevice(const PhysicalDevice&) = delete;
      PhysicalDevice &operator=(const PhysicalDevice&) = delete;

      PhysicalDevice(const PhysicalDevice&&) = delete;
      PhysicalDevice &operator=(const PhysicalDevice&&) = delete;



    private:
      VkInstance m_instance;
      VkSurfaceKHR m_surface;
      VkPhysicalDevice m_GPU_device = VK_NULL_HANDLE;

      // vulkan 1.0-1.2 support very old devices so we do not need to ever check for it these days.
      bool m_GPU_supports_VK_1_3;
      bool m_GPU_supports_VK_1_4;

      SwapchainSupportDetails m_swapchainSupportDetails;
      QueueFamilyIndices m_queueFamilyIndices;



      void chooseDevice(void);
      bool deviceMeetsMinimumRequirements(VkPhysicalDevice device);
      bool areDeviceExtensionsSupported(VkPhysicalDevice device);
      uint32_t giveDeviceQualityRating(VkPhysicalDevice device);
      
      SwapchainSupportDetails querySwapchainSupportDetails(VkPhysicalDevice device);
      QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device);
      
      void getChosenDeviceQualities(void);

      const std::vector<const char *> DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  };

} // namespace venus

#endif // VENUS_PHYSICAL_DEVICE_HPP