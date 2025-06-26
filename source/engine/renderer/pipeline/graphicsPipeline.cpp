#include "graphicsPipeline.hpp"
#include "VN_logger.hpp"
#include "logicalDevice.hpp"
#include "swapchain.hpp"

// STDLIB
#include <bit>
#include <cassert>
#include <fstream>
#include <vector>

namespace venus {
	namespace {  // ANONYMOUS NAMESPACE BEGIN

		struct ShaderStageModules {
			VkShaderModule vertex;
			VkShaderModule fragment;
		};

		auto createShaderModule(const std::string &fileName) -> VkShaderModule {
			std::ifstream file(fileName, std::ios::binary);

			if(!file.is_open()) {
				VN_LOG_CRITICAL("Failed to open shader file.");
				throw std::runtime_error("Failed to open shader file.");
			}

			std::vector<char> rawShaderBytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

			assert(rawShaderBytes.size() % sizeof(uint32_t) == 0);

			const size_t chunkCount = rawShaderBytes.size() / sizeof(uint32_t);
			std::span<const char> rawByteSpan(rawShaderBytes);

			std::vector<uint32_t> shaderByteCode;
			shaderByteCode.reserve(chunkCount);

			for(size_t i = 0; i < chunkCount; ++i) {
				std::array<char, 4> chunk = {};
				std::copy_n(rawByteSpan.subspan(i * 4, 4).begin(), 4, chunk.begin());

				shaderByteCode.push_back(std::bit_cast<uint32_t>(chunk));
			}

			VkShaderModuleCreateInfo createInfo{.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
																					.pNext = nullptr,
																					.flags = 0,
																					.codeSize = (shaderByteCode.size() * sizeof(uint32_t)),
																					.pCode = shaderByteCode.data()};

			VkShaderModule shaderModule = {};
			if(vkCreateShaderModule(volkGetLoadedDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
				VN_LOG_CRITICAL("Failed to create shader module.");
				throw std::runtime_error("Failed to create shader module.");
			}

			VN_LOG_INFO("A shader module has been created.");
			return shaderModule;
		}

		auto createShaderStages(const ShaderStageModules &modules) -> std::vector<VkPipelineShaderStageCreateInfo> {
			VkPipelineShaderStageCreateInfo vertexStageInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
																											.pNext = nullptr,
																											.flags = 0,
																											.stage = VK_SHADER_STAGE_VERTEX_BIT,
																											.module = modules.vertex,
																											.pName = "main",
																											.pSpecializationInfo = nullptr};

			VkPipelineShaderStageCreateInfo fragmentStageInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
																												.pNext = nullptr,
																												.flags = 0,
																												.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
																												.module = modules.fragment,
																												.pName = "main",
																												.pSpecializationInfo = nullptr};

			return {vertexStageInfo, fragmentStageInfo};
		}

	}  // namespace
	// ANONYMOUS NAMESPACE END

	GraphicsPipeline::GraphicsPipeline(const std::shared_ptr<LogicalDevice> &logicalDevicePtr,
																		 const std::shared_ptr<Swapchain> &swapchainPtr):
		m_logicalDevice(logicalDevicePtr), m_swapchain(swapchainPtr) {
		VkShaderModule vertexModule = createShaderModule("shaders/triangle.vert.spv");
		VkShaderModule fragmentModule = createShaderModule("shaders/triangle.frag.spv");
		auto shaderStages = createShaderStages({.vertex = vertexModule, .fragment = fragmentModule});

		std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
			.pDynamicStates = dynamicStates.data(),
		};

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.vertexBindingDescriptionCount = 0,
			.pVertexBindingDescriptions = nullptr,
			.vertexAttributeDescriptionCount = 0,
			.pVertexAttributeDescriptions = nullptr};

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE};

		VkPipelineViewportStateCreateInfo viewportStateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
																												.pNext = nullptr,
																												.flags = 0,
																												.viewportCount = 1,
																												.pViewports = nullptr,
																												.scissorCount = 1,
																												.pScissors = nullptr};

		VkPipelineRasterizationStateCreateInfo rasterStateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0F,
			.depthBiasClamp = 0.0F,
			.depthBiasSlopeFactor = 0.0F,
			.lineWidth = 1.0F};

		VkPipelineMultisampleStateCreateInfo multisampleStateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 1.0F,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE};

		VkPipelineColorBlendAttachmentState colorblendAttachmentStateInfo{
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};

		VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = &colorblendAttachmentStateInfo,
			.blendConstants = {0.0F, 0.0F, 0.0F, 0.0F}};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
																									.pNext = nullptr,
																									.flags = 0,
																									.setLayoutCount = 0,
																									.pSetLayouts = nullptr,
																									.pushConstantRangeCount = 0,
																									.pPushConstantRanges = nullptr};

		if(vkCreatePipelineLayout(m_logicalDevice->getHandle(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) !=
			 VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create pipeline layout.");
			throw std::runtime_error("Failed to create pipeline layout.");
		}

		VkGraphicsPipelineCreateInfo createInfo{.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
																						.pNext = nullptr,
																						.flags = 0,
																						.stageCount = 2,
																						.pStages = shaderStages.data(),
																						.pVertexInputState = &vertexInputInfo,
																						.pInputAssemblyState = &inputAssemblyInfo,
																						.pTessellationState = nullptr,
																						.pViewportState = &viewportStateInfo,
																						.pRasterizationState = &rasterStateInfo,
																						.pMultisampleState = &multisampleStateInfo,
																						.pDepthStencilState = nullptr,
																						.pColorBlendState = &colorBlendStateInfo,
																						.pDynamicState = &dynamicStateInfo,
																						.layout = m_pipelineLayout,
																						.renderPass = m_swapchain->getRenderPass(),
																						.subpass = 0,
																						.basePipelineHandle = VK_NULL_HANDLE,
																						.basePipelineIndex = -1};

		if(vkCreateGraphicsPipelines(m_logicalDevice->getHandle(), VK_NULL_HANDLE, 1, &createInfo, nullptr,
																 &m_graphicsPipeline) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create graphics pipeline.");
			throw std::runtime_error("Failed to create graphics pipeline.");
		}

		// shader modules can be destroyed after being loaded into the pipeline
		vkDestroyShaderModule(m_logicalDevice->getHandle(), vertexModule, nullptr);
		vkDestroyShaderModule(m_logicalDevice->getHandle(), fragmentModule, nullptr);

		VN_LOG_INFO("GraphicsPipeline has been constructued.");
	}

	GraphicsPipeline::~GraphicsPipeline() {
		vkDestroyPipeline(m_logicalDevice->getHandle(), m_graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_logicalDevice->getHandle(), m_pipelineLayout, nullptr);

		VN_LOG_INFO("GraphicsPipeline has been destructed.");
	}

}  // namespace venus
