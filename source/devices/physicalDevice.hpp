#ifndef VENUS_PHYSICAL_DEVICE_HPP
#define VENUS_PHYSICAL_DEVICE_HPP

#include "volk.h"
#include "gpuStructs.hpp"



namespace venus{
  const std::vector<const char *> DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  class PhysicalDevice{
    public:
      PhysicalDevice(const VkInstance& instanceRef, const VkSurfaceKHR& surfaceRef);
      ~PhysicalDevice();

      // class should never be copied.
      PhysicalDevice(const PhysicalDevice&) = delete;
      PhysicalDevice &operator=(const PhysicalDevice&) = delete;
      // class should never be moved.
      PhysicalDevice(const PhysicalDevice&&) = delete;
      PhysicalDevice &operator=(const PhysicalDevice&&) = delete;


      VkPhysicalDevice getDevice(void) const {return m_GPU_device;}
    private:
      VkInstance m_instance;
      VkSurfaceKHR m_surface;
      VkPhysicalDevice m_GPU_device = VK_NULL_HANDLE;
      uint32_t m_API_version = 0;


      void chooseDevice(void);
      bool deviceMeetsMinimumRequirements(VkPhysicalDevice device);
      bool areDeviceExtensionsSupported(VkPhysicalDevice device);
      uint32_t giveDeviceQualityRating(VkPhysicalDevice device);
      
      SwapchainSupportDetails querySwapchainSupportDetails(VkPhysicalDevice device);
      QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device);

      // queries and stores device properties, features, swapchainsupportdetails, queuefamilyindices, etc.
      void storeDeviceCapabilites(void);
      
  };

} // namespace venus

#endif // VENUS_PHYSICAL_DEVICE_HPP