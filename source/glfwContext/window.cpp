#include "window.hpp"
// PROJECT
#include "VN_logger.hpp"

// STDLIB
#include <cassert>
#include <stdexcept>

namespace venus {
	namespace {  // ANONYMOUS NAMESPACE BEGIN

		// glfw uses "int" in their window creation so we will use uin16_t to parse the uint32_t bitmask for resolution values.
		// passing a uint32_t causes implicit narrowing which we would like to avoid, uint16_t has more than enough bits to store resolution values.
		struct resolution {
			uint16_t width;
			uint16_t height;
		};

		auto getDesiredResolution(enum WindowResolution_BIT chosenRes) -> resolution {
			constexpr unsigned int bitShiftLeft_16 = 16;
			constexpr unsigned int bitShiftLowest_pos = 0xFFFF;
			auto defaultResolutionBIT = static_cast<uint32_t>(chosenRes);
			return {.width = static_cast<uint16_t>(defaultResolutionBIT >> bitShiftLeft_16),
							.height = static_cast<uint16_t>(defaultResolutionBIT & bitShiftLowest_pos)};
		}

	}  // namespace

	Window::Window(const AppDetails &detailsRef): m_details(detailsRef) {
		createWindow();

		VN_LOG_INFO("Class::Window, Construction finished.");
	}

	Window::~Window() {
		destroySurface();
		destroyWindow();
		VN_LOG_INFO("Class::Window, Destruction finished.");
	}

	void Window::createWindow() {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // TODO: SET TO GLFW_TRUE AFTER SWAPCHAIN RESIZE IMPLEMENTATION
		auto res = getDesiredResolution(m_details.defaultWindowResolution);
		m_window = glfwCreateWindow(res.width, res.height, m_details.windowTitle, nullptr, nullptr);

		assert(m_window != nullptr);

		m_currentExtent = {.width = res.width, .height = res.height};
		VN_LOG_INFO("Successfully created glfw window.");
	}
	void Window::destroyWindow() {
		if(m_window != nullptr) {
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}
		VN_LOG_INFO("Destroyed glfw window successfully.");
	}

	void Window::createSurface() {
		assert(volkGetLoadedInstance() != nullptr);

		if(glfwCreateWindowSurface(volkGetLoadedInstance(), m_window, nullptr, &m_surface) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create window surface, surface is null.");
			throw std::runtime_error("Failed to create window surface, surface is null.");
		}
		VN_LOG_INFO("Created window surface successfully.");
	}

	void Window::destroySurface() {
		assert(m_surface != VK_NULL_HANDLE);

		vkDestroySurfaceKHR(volkGetLoadedInstance(), m_surface, nullptr);
		VN_LOG_INFO("Destroyed window surface successfully.");
	}

}  // namespace venus
