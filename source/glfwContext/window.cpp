#include "window.hpp"
// PROJECT
#include "VN_logger.hpp"

// STDLIB
#include <cassert>
#include <stdexcept>
#include <string>

namespace venus {
	namespace {  // ANONYMOUS NAMESPACE BEGIN

		// glfw uses "int" in their window creation so we will use uin16_t to parse the uint32_t bitmask for resolution values.
		struct resolution {
			uint16_t width;
			uint16_t height;
		};

		auto getDesiredResolution(enum WindowResolution_BIT chosenRes) -> resolution {
			constexpr auto bitShiftLeft_16 = 16;
			constexpr auto bitShiftLowest_pos = 0xFFFF;
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
		destroyWindow();
		glfwTerminate();
		VN_LOG_INFO("Class::Window, Destruction finished.");
	}

	void Window::createWindow() {
		if(glfwInit() != GLFW_TRUE) {
			const std::string glfw_fail_msg = "Failed to initialize GLFW.";
			VN_LOG_CRITICAL(glfw_fail_msg);
			throw std::runtime_error(glfw_fail_msg);
		}

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
		}
		m_window = nullptr;
		VN_LOG_INFO("Destroyed glfw window successfully.");
	}

	void Window::createSurface(VkInstance instance) {
		assert(instance != nullptr);

		glfwCreateWindowSurface(instance, m_window, nullptr, &m_surface);

		if(m_surface == VK_NULL_HANDLE) {
			const std::string surface_null_message = "Failed to create window surface, surface is null.";
			VN_LOG_CRITICAL(surface_null_message);
			throw std::runtime_error(surface_null_message);
		}

		VN_LOG_INFO("Created window surface successfully.");
	}

	void Window::destroySurface(VkInstance instance) {
		assert(instance != nullptr);

		if(m_surface != nullptr) {
			auto func = reinterpret_cast<PFN_vkDestroySurfaceKHR>(vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR"));
			if(func != nullptr) {
				func(instance, m_surface, nullptr);
				m_surface = nullptr;
				VN_LOG_INFO("Destroyed window surface successfully.");
			} else {
				const std::string func_error_msg =
					"(vkGetInstanceProcAddr) Failed to acquire (PFN_vkDestroySurfaceKHR): function pointer is null.";
				VN_LOG_WARN(func_error_msg);
				throw std::runtime_error(func_error_msg);
			}
		}
	}

}  // namespace venus
