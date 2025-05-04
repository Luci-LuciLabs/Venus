#ifndef VENUS_SWAPCHAIN_SUPPORT_DETAILS_HPP
#define VENUS_SWAPCHAIN_SUPPORT_DETAILS_HPP

#include "volk.h"

// STDLIB
#include <optional>
#include <vector>

namespace venus::rendersystem{

  struct appDetails {
    char* appName;
    int appVersionMajour;
    int appVersionMinour;
    int appVersionPatch;
  };

  /// @brief Holds chosen device extensions. Used by both physical and logical devices.
  const std::vector<const char *> DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	/// @brief Holds physical device swapchain support details. Used by both physical and logical devices.
	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> surfaceFormats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	/// @brief Holds physical device queue family indices. Used by both physical and logical devices.
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

  /// @brief Holds nearly all required pipeline configuration structures.
  struct PipelineConfigInfo{
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
    VkPipelineColorBlendAttachmentState colourBlendAttachmentState;
    VkPipelineColorBlendStateCreateInfo colourBlendStateCreateInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;

    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
  };

} // namespace venus::rendersystem

#endif // VENUS_SWAPCHAIN_SUPPORT_DETAILS_HPP