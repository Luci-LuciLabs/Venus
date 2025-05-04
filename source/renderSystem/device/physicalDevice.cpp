#include "physicalDevice.hpp"

// STDLIB
#include <stdexcept>
#include <map>
#include <set>
#include <string>
#include <cassert>

namespace venus::rendersystem {

	physicalDevice::physicalDevice() {}
	physicalDevice::~physicalDevice() {}

	bool physicalDevice::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
    assert(instance != nullptr && "cant pick gpu if instance is null.");
    assert(surface != nullptr &&"cant pick gpu if surface is null.");

    bool deviceHasBeenChosen = false;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if(deviceCount == 0) {
      deviceHasBeenChosen = false;
			throw std::runtime_error("Failed to find any device that supports vulkan.");
		}
		std::vector<VkPhysicalDevice> availableDevices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, availableDevices.data());

		std::multimap<uint32_t, VkPhysicalDevice> scoredDevices;
		for(const auto &device : availableDevices) {
			if(deviceMeetsMinimumRequired(device, surface)) {
				uint32_t score = giveFoundDeviceScore(device);
				scoredDevices.insert(std::make_pair(score, device));
			}
		}

		if(scoredDevices.rbegin()->first > 0) {
			m_physicalDevice = scoredDevices.rbegin()->second;
		} else {
      deviceHasBeenChosen = false;
			throw std::runtime_error("Failed to find any suitable GPU.");
		}

		if(m_physicalDevice == VK_NULL_HANDLE) {
      deviceHasBeenChosen = false;
			throw std::runtime_error("Failed to properly acquire physical device.");
		}else{
      deviceHasBeenChosen = true;
    }

		m_queueFamilyIndices = findQueueFamilyIndices(m_physicalDevice, surface);
		m_swapchainSupportDetails = querySwapchainSupportDetails(m_physicalDevice, surface);
    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_deviceFeatures);

    return deviceHasBeenChosen;
	}

	bool physicalDevice::deviceMeetsMinimumRequired(VkPhysicalDevice device, VkSurfaceKHR surface) {
		QueueFamilyIndices indices = findQueueFamilyIndices(device, surface);

		bool extensionsAreSupported = areDeviceExtensionsSupported(device);

		bool swapchainIsAdequate = false;
		if(extensionsAreSupported) {
			SwapchainSupportDetails swapchainDetails = querySwapchainSupportDetails(device, surface);
			swapchainIsAdequate = !swapchainDetails.surfaceFormats.empty() && !swapchainDetails.presentModes.empty();
		}

		return indices.isComplete() && swapchainIsAdequate && extensionsAreSupported;
	}

	bool physicalDevice::areDeviceExtensionsSupported(VkPhysicalDevice device) {
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

	uint32_t physicalDevice::giveFoundDeviceScore(VkPhysicalDevice device) {
		uint32_t device_totalScore = 0;

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			device_totalScore += 1000;
		}

		device_totalScore += deviceProperties.limits.maxImageDimension2D;

		if(!deviceFeatures.geometryShader) {
			return 0;
		}

		return device_totalScore;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SwapchainSupportDetails physicalDevice::querySwapchainSupportDetails(VkPhysicalDevice physical_device,
																																			 VkSurfaceKHR surface) {
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.surfaceCapabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, nullptr);

		if(formatCount != 0) {
			details.surfaceFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, details.surfaceFormats.data());
		}

		uint32_t presentModes;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModes, nullptr);

		if(presentModes != 0) {
			details.presentModes.resize(presentModes);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModes, details.presentModes.data());
		}

		return details;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	QueueFamilyIndices physicalDevice::findQueueFamilyIndices(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
		QueueFamilyIndices indices;

		// get queueFamilyCount for queueFamilies vector
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, nullptr);
		// supply queueFamilies vector with our device queues data
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, queueFamilies.data());

		// index for queueFamilyIndices, DO NOT CHANGE.
		int i = 0;
		// for each queueFamily found in the queueFamilies vector we will loop.
		for(const auto &queueFamily : queueFamilies) {
			// check if indices is complete
			if(indices.isComplete()) {
				break;
			}

			// check if graphics queue has been found
			if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			// check if present queue is supported
			VkBool32 presentQueueSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &presentQueueSupport);
			if(queueFamily.queueCount > 0 && presentQueueSupport) {
				indices.presentFamily = i;
			}

			i++;
		}

		// this function should ALWAYS RETURN indices, DO NOT CHANGE.
		return indices;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}  // namespace venus::rendersystem
