#include "physicalDevice.hpp"
#include "VN_logger.hpp"

// STDLIB
#include <stdexcept>
#include <string>
#include <map>
#include <set>

namespace venus {

	PhysicalDevice::PhysicalDevice(const VkInstance& instanceRef, const VkSurfaceKHR& surfaceRef) : m_instance(instanceRef), m_surface(surfaceRef) {
    VN_LOG_INFO("Constructing PhysicalDevice.");
    chooseDevice();
  }
	PhysicalDevice::~PhysicalDevice() {}

	void PhysicalDevice::chooseDevice(void) {
		bool deviceHasBeenChosen = false;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
		if(deviceCount == 0) {
			VN_LOG_CRITICAL("Failed to find any graphics device.");
			throw std::runtime_error("Failed to find any graphics device.");
		}
		std::vector<VkPhysicalDevice> availableDevices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, availableDevices.data());

		std::multimap<uint32_t, VkPhysicalDevice> scoredDevices;
		for(const auto &device : availableDevices) {
			if(deviceMeetsMinimumRequirements(device)) {
				uint32_t rating = giveDeviceQualityRating(device);
				scoredDevices.insert(std::make_pair(rating, device));
			}
		}

    if(scoredDevices.rbegin()->first > 0){
      m_GPU_device = scoredDevices.rbegin()->second;
      deviceHasBeenChosen = true;
    }else{
      deviceHasBeenChosen = false;
    }

    if(deviceHasBeenChosen == false){
      VN_LOG_CRITICAL("No GPU was able to be selected, did not pass minimum requirements.");
      throw std::runtime_error("No GPU was able to be selected, did not pass minimum requirements.");
    }
    
    if(m_GPU_device == VK_NULL_HANDLE){
      VN_LOG_CRITICAL("GPU chosen but still VK_NULL_HANDLE.");
      throw std::runtime_error("GPU chosen but still VK_NULL_HANDLE.");
    }


    VN_LOG_INFO("GPU has been selected.");
	}

	bool PhysicalDevice::deviceMeetsMinimumRequirements(VkPhysicalDevice device) {
		QueueFamilyIndices indices = findQueueFamilyIndices(device);

		bool extensionsAreSupported = areDeviceExtensionsSupported(device);
		bool swapchainIsAdequate = false;
		if(extensionsAreSupported) {
			SwapchainSupportDetails details = querySwapchainSupportDetails(device);
			swapchainIsAdequate = !details.surfaceFormats.empty() && !details.presentModes.empty();
		}

		return indices.supportsMinimum() && swapchainIsAdequate && extensionsAreSupported;
	}

	bool PhysicalDevice::areDeviceExtensionsSupported(VkPhysicalDevice device) {
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());

		for(const auto &extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	uint32_t PhysicalDevice::giveDeviceQualityRating(VkPhysicalDevice device) {
		uint32_t deviceRating = 0;

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if(!deviceFeatures.geometryShader) {
			return 0;
		}
		uint32_t dev_maj = VK_API_VERSION_MAJOR(deviceProperties.apiVersion);
		uint32_t dev_min = VK_API_VERSION_MINOR(deviceProperties.apiVersion);
		uint32_t dev_pat = VK_API_VERSION_PATCH(deviceProperties.apiVersion);
    std::string dev_name = deviceProperties.deviceName;
		std::string vk_device_version_msg =
			"[" + dev_name + "]" + " Vulkan-Version: " + std::to_string(dev_maj) + "." + std::to_string(dev_min) + "." + std::to_string(dev_pat) + '\n';
		VN_LOG_INFO(vk_device_version_msg);

		uint32_t instanceVersion = 0;
		vkEnumerateInstanceVersion(&instanceVersion);
		uint32_t ins_maj = VK_API_VERSION_MAJOR(instanceVersion);
		uint32_t ins_min = VK_API_VERSION_MINOR(instanceVersion);

		if(deviceProperties.apiVersion < VK_MAKE_VERSION(ins_maj, ins_min, 0)) {
			std::string vk_device_version_warn =
				"[" + dev_name + "]" + " : Device API version is inadequate for chosen instance version.\n";
			VN_LOG_WARN(vk_device_version_msg);
			return 0;
		} else {
			deviceRating += 10000;
		}

		if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			deviceRating += 10000;
		}

		deviceRating += deviceProperties.limits.maxImageDimension2D;

		return deviceRating;
	}

	SwapchainSupportDetails PhysicalDevice::querySwapchainSupportDetails(VkPhysicalDevice device) {
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.surfaceCapabilities);
		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
		if(formatCount != 0) {
			details.surfaceFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.surfaceFormats.data());
		}
		uint32_t presentModes = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModes, nullptr);
		if(presentModes != 0) {
			details.presentModes.resize(presentModes);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModes, details.presentModes.data());
		}

		return details;
	}
	QueueFamilyIndices PhysicalDevice::findQueueFamilyIndices(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;

		for(const auto &queueFamily : queueFamilies) {
			if(indices.supportsMinimum()) {
				break;
			}

			if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentQueueSupported = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentQueueSupported);
			if(queueFamily.queueCount > 0 && presentQueueSupported) {
				indices.presentFamily = i;
			}

			if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
				indices.computeFamily = i;
			}

			i++;
		}
    return indices;
	}



  void PhysicalDevice::storeDeviceCapabilites(void){
    


  }



}  // namespace venus