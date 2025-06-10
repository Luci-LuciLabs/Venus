#ifndef VENUS_GLFW_WINDOW_HPP
#define VENUS_GLFW_WINDOW_HPP

// project
#include "appDetails.hpp"

// THIRD PARTY
#include "volk.h"
// this define prevents glfw from including any opengl specific content.
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace venus {
	class Window {
	public:
		Window(const AppDetails &detailsRef);
		~Window();

		Window(const Window &) = delete;
		auto operator=(const Window &) -> Window & = delete;
		Window(const Window &&) = delete;
		auto operator=(const Window &&) -> Window & = delete;

		[[nodiscard]] auto shouldClose() const { return static_cast<bool>(glfwWindowShouldClose(m_window)); }
		[[nodiscard]] auto getSurfaceHandle() const { return m_surface; }
		[[nodiscard]] auto getAppDetails() const { return m_details; }

		void createSurface();
		void destroySurface();

	private:
		AppDetails m_details;
		GLFWwindow *m_window = nullptr;

		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
		VkExtent2D m_currentExtent = {};

		void createWindow();
		void destroyWindow();
	};

}  // namespace venus

#endif  // VENUS_GLFW_WINDOW_HPP