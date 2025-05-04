#include "logicalDevice.hpp"

// STDLIB
#include <set>
#include <optional>
#include <iostream>

namespace venus::rendersystem{
  
  logicalDevice::logicalDevice(){}
  logicalDevice::~logicalDevice(){}

  bool logicalDevice::createLogicalDevice(VkPhysicalDevice physicalDevice,
                                          VkPhysicalDeviceFeatures deviceFeatures, QueueFamilyIndices indices){

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfosVector;                                        
    std::set<std::optional<uint32_t>> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

    const float queuePriority = 1.0f;

    for(std::optional<uint32_t> queueFamily : uniqueQueueFamilies){
      VkDeviceQueueCreateInfo queueCreateInfo = {};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily.value();
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      
      queueCreateInfosVector.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfosVector.size());
    createInfo.pQueueCreateInfos = queueCreateInfosVector.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

    if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS){
      return false;
    }

    volkLoadDevice(m_logicalDevice);

    vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    if(m_graphicsQueue == VK_NULL_HANDLE){
      std::cout << "graphics queue failed.\n";
      return false;
    }

    vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &m_presentQueue);
    if(m_presentQueue == VK_NULL_HANDLE){
      std::cout << "present queue failed.\n";
      return false;
    }


    return true;
  }

  void logicalDevice::destroyLogicalDevice(void){
    if(m_logicalDevice != VK_NULL_HANDLE){
      vkDestroyDevice(m_logicalDevice, nullptr);
    }
    m_logicalDevice = VK_NULL_HANDLE;
    
  }
  
} // namespace venus::rendersystem
