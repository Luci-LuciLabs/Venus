#include "window.hpp"
// PROJECT
#include "VN_logger.hpp"
#include "keyboardInput.hpp"

// STDLIB
#include <cassert>
#include <stdexcept>

namespace venus {
	namespace {  // ANONYMOUS NAMESPACE BEGIN
		// glfw uses "int" in their window creation so we will use uin16_t to parse the uint32_t bitmask for resolution values.
		// passing a uint32_t causes implicit narrowing which we would like to avoid, uint16_t has more than enough bits to store resolution values.
		struct WindowResolution {
			uint16_t width;
			uint16_t height;
		};

		auto getDesiredResolution(const WindowConfigDetails &details) -> WindowResolution {
			constexpr unsigned int bitShiftLeft_16 = 16;
			constexpr unsigned int bitShiftLowest_pos = 0xFFFF;

			WindowResolution CHOSEN_RES = {.width = static_cast<uint16_t>(details.ResolutionBit >> bitShiftLeft_16),
																		 .height = static_cast<uint16_t>(details.ResolutionBit & bitShiftLowest_pos)};

			WindowResolution MIN_RES{};
			if(details.AspectRatioFlag == ASPECT_RATIO_4_BY_3_FLAG_BIT) {
				MIN_RES = {.width = static_cast<uint16_t>(RESOLUTION_4x3_VGA_BIT >> bitShiftLeft_16),
									 .height = static_cast<uint16_t>(RESOLUTION_4x3_VGA_BIT & bitShiftLowest_pos)};
			} else {
				MIN_RES = {.width = static_cast<uint16_t>(RESOLUTION_16x9_FWVGA_BIT >> bitShiftLeft_16),
									 .height = static_cast<uint16_t>(RESOLUTION_16x9_FWVGA_BIT & bitShiftLowest_pos)};
			}

			if(details.ResolutionBit == RESOLUTION_NATIVE_BIT &&
				 ((details.WindowModeFlag & WINDOW_MODE_NORMAL_FLAG_BIT) != 0U)) {
				VN_LOG_WARN("Venus does not allow native resolution for normal windows, defaulting to 480p.");
				return MIN_RES;
			}

			if(CHOSEN_RES.width < MIN_RES.width || CHOSEN_RES.height < MIN_RES.height) {
				VN_LOG_WARN("Default resolution set below minimum supported, defaulting to 480p.");
				return MIN_RES;
			}

			return CHOSEN_RES;
		}

	}  // namespace

	Window::Window(const WindowConfigDetails &configDetails): m_details(configDetails) {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		if((m_details.WindowModeFlag & WINDOW_MODE_NORMAL_FLAG_BIT) != 0) {
			createNormalWindow();
			VN_LOG_INFO("Using Window-Mode: Normal.");
		}
		if((m_details.WindowModeFlag & WINDOW_MODE_BORDERLESS_FLAG_BIT) != 0) {
			createBorderlessWindow();
			VN_LOG_INFO("Using Window-Mode: Borderless.");
		}
		if((m_details.WindowModeFlag & WINDOW_MODE_FULLSCREEN_FLAG_BIT) != 0) {
			createFullscreenWindow();
			VN_LOG_INFO("Using Window-Mode: Fullscreen.");
		}

		createSurface();

		glfwSetKeyCallback(m_window, key_callback);

		VN_LOG_INFO("Venus Window has been created.");
	}

	Window::~Window() {
		destroySurface();
		destroyWindow();
		VN_LOG_INFO("Venus Window has been destroyed.");
	}

	void Window::createNormalWindow() {
		auto resolution = getDesiredResolution(m_details);
		m_window = glfwCreateWindow(resolution.width, resolution.height, m_details.title, nullptr, nullptr);
		assert(m_window != nullptr);
	}

	void Window::createFullscreenWindow() {
		auto resolution = getDesiredResolution(m_details);
		m_window = glfwCreateWindow(resolution.width, resolution.height, m_details.title, m_monitor, nullptr);
		assert(m_window != nullptr);
	}

	void Window::createBorderlessWindow() {
		auto resolution = getDesiredResolution(m_details);
		m_window = glfwCreateWindow(resolution.width, resolution.height, m_details.title, nullptr, nullptr);
		glfwSetWindowMonitor(m_window, m_monitor, 0, 0, resolution.width, resolution.height, GLFW_DONT_CARE);
		assert(m_window != nullptr);
	}

	void Window::destroyWindow() {
		if(m_window != nullptr) {
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}
		VN_LOG_INFO("window has been destroyed.");
	}

	void Window::createSurface() {
		assert(volkGetLoadedInstance() != nullptr);

		if(glfwCreateWindowSurface(volkGetLoadedInstance(), m_window, nullptr, &m_surface) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create window surface, surface is null.");
			throw std::runtime_error("Failed to create window surface, surface is null.");
		}
		VN_LOG_INFO("Window surface has been created.");
	}

	void Window::destroySurface() {
		assert(m_surface != VK_NULL_HANDLE);

		vkDestroySurfaceKHR(volkGetLoadedInstance(), m_surface, nullptr);
		VN_LOG_INFO("Window surface has been destroyed.");
	}

	void Window::changeResolution(uint16_t width, uint16_t height) { glfwSetWindowSize(m_window, width, height); }

	auto Window::getCurrentSurfaceExtent() -> VkExtent2D {
		int glfw_width = 0;
		int glfw_height = 0;

		glfwGetFramebufferSize(m_window, &glfw_width, &glfw_height);

		return {.width = static_cast<uint32_t>(glfw_width), .height = static_cast<uint32_t>(glfw_height)};
	}

}  // namespace venus
