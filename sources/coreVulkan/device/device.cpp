#include "device.hpp"
#include "deviceConfig.hpp"
#include "queueFamilyIndices.hpp"
#include "swapchainSupportDetails.hpp"

// stdlib
#include <cstdint>
#include <stdexcept>
#include <map>
#include <set>
#include <unordered_set>
#include <iostream>

namespace venus_core{
    
    device_core::device_core( instance_core  &instance_coreRef, window_core &window_coreRef) : m_instance{instance_coreRef}, m_window{window_coreRef}{
        pickPhysicalDevice();
        createLogicalDevice();
    };

    device_core::~device_core(){
        if(m_logicalDevice != VK_NULL_HANDLE){
            vkDestroyDevice(m_logicalDevice, nullptr);
        }
        m_logicalDevice = VK_NULL_HANDLE;

        std::cout << "physicalDevice destroyed." << '\n';
    };

    void device_core::pickPhysicalDevice(){
        // DO NOT CHANGE initial count.
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance.getInstance(), &deviceCount, nullptr);

        if(deviceCount == 0){
            throw std::runtime_error("FAILED TO FIND ANY PHYSICAL DEVICE!!!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance.getInstance(), &deviceCount, devices.data());

        // we create an empty multimap here in order to provide pairs of scores and devices within the loop below
        std::multimap<int, VkPhysicalDevice> deviceCandidates;

        for(const auto &device : devices){
            if(isDeviceSuitable(device)){
                int score = ratePhysicalDeviceSuitability(device);
                deviceCandidates.insert(std::make_pair(score, device));
            }
        }

        if(deviceCandidates.rbegin()->first > 0){
            m_physicalDevice = deviceCandidates.rbegin()->second;
        }else{
            throw std::runtime_error("FAILED TO FIND GPU THAT SUPPORTS VULKAN ADEQUATELY!!!");
        }

        if(m_physicalDevice == VK_NULL_HANDLE){
            throw std::runtime_error("FAILED TO PICK PHYSICAL DEVICE!!!");
        }
    };

    bool device_core::isDeviceSuitable(VkPhysicalDevice physicalDevice){
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, m_window.getSurface());

        bool extensionsSupported = areRequiredExtensionsSupported(physicalDevice);
        bool swapchainSupportAdequate = false;

        if(extensionsSupported){
            SwapchainSupportDetails swapchainDetails = querySwapchainSupportDetails(physicalDevice, m_window.getSurface());
            swapchainSupportAdequate = !swapchainDetails.surfaceFormats.empty() && !swapchainDetails.surfaceFormats.empty();
        }

        return indices.isComplete() && swapchainSupportAdequate && extensionsSupported;
    };

    bool device_core::areRequiredExtensionsSupported(VkPhysicalDevice physicalDevice){
        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());

        for(const auto &extension : availableExtensions){
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    };

    int device_core::ratePhysicalDeviceSuitability(VkPhysicalDevice physicalDevice){
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

        int score = 0;

        if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            score += 1000;
        }

        score += deviceProperties.limits.maxImageDimension2D;

        if(!deviceFeatures.geometryShader){
            return 0;
        }

        return score;
    };


    void device_core::createLogicalDevice(void){

        QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice, m_window.getSurface());
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(m_physicalDevice, &deviceFeatures);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        std::set<std::optional<uint32_t>> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

        float queuePriority = 1.0;

        for(std::optional<uint32_t> queueFamily : uniqueQueueFamilies){
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily.value();
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo logicalDeviceCreateInfo = {};
        logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        logicalDeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        logicalDeviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

        logicalDeviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        logicalDeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
        logicalDeviceCreateInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

        if(vkCreateDevice(m_physicalDevice, &logicalDeviceCreateInfo, nullptr, &m_logicalDevice) != VK_SUCCESS){
            throw std::runtime_error("FAILED TO CREATE LOGICAL DEVICE!!!");
        }
        
        volkLoadDevice(m_logicalDevice);

        vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
        if(graphicsQueue == VK_NULL_HANDLE){
            throw std::runtime_error("FAILED TO ALLOCATE GRAPHICS QUEUE!!!");
        }
        vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
        if(presentQueue == VK_NULL_HANDLE){
            throw std::runtime_error("FAILED TO ALLOCATE PRESENT QUEUE!!!");
        }

        std::cout << "logicalDevice created successfully." << '\n';
    }






} // namespace venus_core
