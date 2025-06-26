#ifndef VENUS_GPU_STRUCTURES_HPP
#define VENUS_GPU_STRUCTURES_HPP

// THIRD PARTY
#include "volk.h"

// STDLIB
#include <optional>
#include <vector>

namespace venus {

	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR supportedSurfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> supportedSurfaceFormats;
		std::vector<VkPresentModeKHR> supportedPresentModes;
	};

	// NOLINTBEGIN
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamilyIndex;
		std::optional<uint32_t> presentFamilyIndex;
		bool supportsMinimum() { return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value(); }
	};
	// NOLINTEND

}  // namespace venus

#endif  // VENUS_GPU_STRUCTURES_HPP