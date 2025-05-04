#include "renderSystem.hpp"

// STDLIB
#include <stdexcept>

namespace venus::rendersystem {

	renderSystem::renderSystem() { initRenderSystem(); }
	renderSystem::~renderSystem() { killRenderSystem(); }

	void renderSystem::runLoop(void) {
		while(!renderWindow->shouldClose()) {
			glfwPollEvents();
		}
	}

	void renderSystem::initRenderSystem(void) {
		renderInstance->createInstance();

		if(!renderWindow->createWindow()) {
			throw std::runtime_error("FAIILED TO CREATE WINDOW!");
		}
		if(!renderWindow->createSurface(renderInstance->getInstance())) {
			throw std::runtime_error("FAILED TO CREATE WINDOW SURFACE!");
		}
		if(!renderPhysicalDevice->pickPhysicalDevice(renderInstance->getInstance(), renderWindow->getSurface())) {
			throw std::runtime_error("FAILED TO PICK DEVICE!");
		}
		if(!renderLogicalDevice->createLogicalDevice(renderPhysicalDevice->getDevice(), renderPhysicalDevice->getFeatures(),
																								 renderPhysicalDevice->getQueueFamilyIndices())) {
			throw std::runtime_error("FAILED TO CREATE LOGICAL DEVICE!");
		}
		if(!renderSwapchain->createSwapchain(renderLogicalDevice->getLogicalDevice(), renderWindow->getSurface(),
																				 renderWindow->getExtent(), renderPhysicalDevice->getSwapchainSupportDetails(),
																				 renderPhysicalDevice->getQueueFamilyIndices()))
    {
      throw std::runtime_error("FAILED TO CREATE SWAPCHAIN!");
    }
	}

	void renderSystem::killRenderSystem(void) {
    renderSwapchain->destroySwapchain(renderLogicalDevice->getLogicalDevice());
		renderLogicalDevice->destroyLogicalDevice();
		renderWindow->destroySurface(renderInstance->getInstance());
		renderWindow->destroyWindow();
		renderInstance->destroyInstance();
	}

}  // namespace venus::rendersystem
