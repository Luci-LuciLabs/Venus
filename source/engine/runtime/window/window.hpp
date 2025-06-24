#ifndef VENUS_GLFW_WINDOW_HPP
#define VENUS_GLFW_WINDOW_HPP

// project
#include "appConfigOptions.hpp"

// THIRD PARTY
#include "volk.h"
// this define prevents glfw from including any opengl specific content.
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace venus {
	/**
   * @brief A window-system-integration object.
   *
   * @class Window
   *
   * @details This object manages the application window, the window display surface, and window events.
   *          This object cannot be copied. This object cannot be moved.
   * 
   *          Creates different windows with different aspect ratios and resolutions depending upon the bit flags in configDetails.
   */
	class Window {
	public:
		explicit Window(const WindowConfigDetails &configDetails);
		~Window();

		Window(const Window &) = delete;
		auto operator=(const Window &) -> Window & = delete;
		Window(const Window &&) = delete;
		auto operator=(const Window &&) -> Window & = delete;

		[[nodiscard]] auto shouldClose() const { return static_cast<bool>(glfwWindowShouldClose(m_window)); }
		[[nodiscard]] auto getSurfaceHandle() const { return m_surface; }

	private:
		WindowConfigDetails m_details;
		GLFWwindow *m_window = nullptr;

		GLFWmonitor *m_monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *m_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
		VkExtent2D m_currentExtent = {};

		void createNormalWindow();
		void createFullscreenWindow();
		void createBorderlessWindow();
		void forceMinimumSize();

		void destroyWindow();

		void createSurface();
		void destroySurface();

		void changeResolution(uint16_t width, uint16_t height);
		void makeWindowFullscreenBorderless();
	};

}  // namespace venus

#endif  // VENUS_GLFW_WINDOW_HPP