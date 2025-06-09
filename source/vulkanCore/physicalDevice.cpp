#include "physicalDevice.hpp"
#include "VN_logger.hpp"

// STDLIB
#include <cassert>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>

namespace venus {
	// ANONYMOUS NAMESPACE BEGIN
	namespace {

		auto areDeviceExtensionsSupported(VkPhysicalDevice device) -> bool {
			assert(device != nullptr);

			VN_LOG_DEBUG("Checking if graphics device supports required extensions.");

			uint32_t extensionCount = 0;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> extensions_required(REQUIRED_EXTENSIONS.begin(), REQUIRED_EXTENSIONS.end());
			for(const auto &extension : availableExtensions) {
				extensions_required.erase(static_cast<const char *>(extension.extensionName));
			}

			return extensions_required.empty();
		}

		auto querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) -> SwapchainSupportDetails {
			assert(device != nullptr);
			assert(surface != nullptr);

			VN_LOG_DEBUG("Querying graphics device swapchain support details.");

			SwapchainSupportDetails details = {};

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.supportedSurfaceCapabilities);

			uint32_t formatCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
			if(formatCount != 0) {
				details.supportedSurfaceFormats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.supportedSurfaceFormats.data());
			} else {
				VN_LOG_CRITICAL("Failed to find any supported surface formats.");
				throw std::runtime_error("Failed to find any supported surface formats.");
			}

			uint32_t presentCount = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, nullptr);
			if(presentCount != 0) {
				details.supportedPresentModes.resize(presentCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, details.supportedPresentModes.data());
			} else {
				VN_LOG_CRITICAL("Failed to find any supported presentation modes.");
				throw std::runtime_error("Failed to find any supported presentation mode.");
			}

			return details;
		}

		auto findQueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface) -> QueueFamilyIndices {
			assert(device != nullptr);
			assert(surface != nullptr);

			VN_LOG_DEBUG("Finding graphics device queue family indices.");

			QueueFamilyIndices indices = {};

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> familyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, familyProperties.data());

			uint32_t INDEX_NUMBER = 0;
			for(const auto &family : familyProperties) {
				// TODO: Update this to properly determine which queue families we want to use.
				if(indices.supportsMinimum()) {
					break;
				}

				bool familySupportsGraphics = static_cast<bool>(family.queueFlags & VK_QUEUE_GRAPHICS_BIT);
				if(family.queueCount > 0 && familySupportsGraphics) {
					indices.graphicsFamilyIndex = INDEX_NUMBER;
				}

				bool familySupportsCompute = static_cast<bool>(family.queueFlags & VK_QUEUE_COMPUTE_BIT);
				if(family.queueCount > 0 && familySupportsCompute) {
					indices.computeFamilyIndex = INDEX_NUMBER;
				}

				bool familySupportsTransfer = static_cast<bool>(family.queueFlags & VK_QUEUE_TRANSFER_BIT);
				if(family.queueCount > 0 && familySupportsTransfer) {
					indices.transferFamilyIndex = INDEX_NUMBER;
				}

				VkBool32 presentationIsSupported = 0U;  // SET TO FALSE
				vkGetPhysicalDeviceSurfaceSupportKHR(device, INDEX_NUMBER, surface, &presentationIsSupported);
				if(family.queueCount > 0 && (presentationIsSupported != 0U)) {
					indices.presentFamilyIndex = INDEX_NUMBER;
				}

				++INDEX_NUMBER;
			}

			return indices;
		}

		auto reportDeviceScore(VkPhysicalDevice device) -> uint32_t {
			constexpr uint32_t DISCRETE_GPU_BONUS_SCORE = 10000;

			VN_LOG_DEBUG("Scoring graphics device suitability");

			uint32_t TOTAL_SCORE = 0;
			VkPhysicalDeviceProperties properties;

			vkGetPhysicalDeviceProperties(device, &properties);
			if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				TOTAL_SCORE = DISCRETE_GPU_BONUS_SCORE;
			}

			// TODO: actually score the devices, currently just giving 10k points to discrete gpus

			return TOTAL_SCORE;
		}

		auto meetsMinimumRequirements(VkPhysicalDevice device, VkSurfaceKHR surface) -> bool {
			assert(device != nullptr);
			assert(surface != nullptr);
			VN_LOG_DEBUG("Checking if graphics device meets minimum requirements.");

			if(!areDeviceExtensionsSupported(device)) {
				VN_LOG_WARN("This device does not support required extensions.");
				return false;
			}
			QueueFamilyIndices indices = findQueueFamilyIndices(device, surface);
			if(!indices.supportsMinimum()) {
				VN_LOG_WARN("This device does not support minimum required queue families.");
				return false;
			}
			SwapchainSupportDetails details = querySwapchainSupport(device, surface);
			return !(details.supportedSurfaceFormats.empty() || details.supportedPresentModes.empty());
		}

	}  // namespace
	// ANONYMOUS NAMESPACE END

	PhysicalDevice::PhysicalDevice(const VkSurfaceKHR &surfaceRef) {
		assert(surfaceRef != nullptr);

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(volkGetLoadedInstance(), &deviceCount, nullptr);
		if(deviceCount == 0) {
			VN_LOG_CRITICAL("Failed to find any graphics device.");
			throw std::runtime_error("Failed to find any graphics device.");
		}
		std::vector<VkPhysicalDevice> devicesFound(deviceCount);
		vkEnumeratePhysicalDevices(volkGetLoadedInstance(), &deviceCount, devicesFound.data());

		std::multimap<uint32_t, VkPhysicalDevice> sortedDevices;
		for(const auto &possibleDevice : devicesFound) {
			if(meetsMinimumRequirements(possibleDevice, surfaceRef)) {
				uint32_t deviceScore = reportDeviceScore(possibleDevice);
				sortedDevices.insert(std::make_pair(deviceScore, possibleDevice));
			}
		}

		if(sortedDevices.rbegin()->first > 0) {
			m_gpuDevice = sortedDevices.rbegin()->second;
		} else {
			VN_LOG_CRITICAL("Failed to successfully choose a graphics device.");
			throw std::runtime_error("failed to successfully choose a graphics device.");
		}

		if(m_gpuDevice == VK_NULL_HANDLE) {
			VN_LOG_CRITICAL("Failed to successfully acquire graphics device.");
			throw std::runtime_error("Failed to successfully acquire graphics device.");
		}

		m_gpuDevice_queueFamilyIndices = findQueueFamilyIndices(m_gpuDevice, surfaceRef);
		m_gpuDevice_swapchainSupportDetails = querySwapchainSupport(m_gpuDevice, surfaceRef);

		VN_LOG_INFO("PhysicalDevice acquisition was completed successfully.");
	}

	PhysicalDevice::~PhysicalDevice() { VN_LOG_INFO("Physical Device was released successfully"); }

}  // namespace venus