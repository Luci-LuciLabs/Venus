#ifndef VENUS_COMMON_GPU_STRUCTS_HPP
#define VENUS_COMMON_GPU_STRUCTS_HPP

#include "volk.h"

// STDLIB
#include <optional>
#include <vector>

struct SwapchainSupportDetails {
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> computeFamily;
	bool supportsMinimum() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	bool supportsCompute() { return computeFamily.has_value(); }
};


#endif  // VENUS_COMMON_GPU_STRUCTS_HPP