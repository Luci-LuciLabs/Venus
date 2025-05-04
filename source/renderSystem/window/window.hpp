#ifndef VENUS_WINDOW_HPP
#define VENUS_WINDOW_HPP

#include "volk.h"
#include <GLFW/glfw3.h>

// STDLIB
#include <string>

namespace venus::rendersystem {
	
  /// @brief Controls glfw window, window surface, and window events.
  ///
  /// @details
  /// This class should only ever by called AFTER a valid vulkan instance has been created.
  /// Although the window class requires glfw it does not initialize it, the instance class handles it instead.
  ///
  /// The creation and destruction of a glfw window, with not API context.
  ///
  /// This class handles the creation and destructions of a vulkan window surface.
  /// 
  class window {
	public:

	  /// @brief This class takes no construction members, empty constructor.
	  window();
		/// @brief Empty destructor.
		~window();

		/// @brief This class should never be copied. 
		window(const window &) = delete;
		/// @brief This class should never be copied. 
		window &operator=(const window &) = delete;

		/// @brief Class getter for glfwWindowShouldClose(). Requires void to get member.
		/// @param void 
		/// @return bool
		bool shouldClose(void) const {return glfwWindowShouldClose(m_window);}
    
    /// @brief Class getter for class member surface. Requires void to get member.
    /// @param  void
    /// @return VKSurfaceKHR
    VkSurfaceKHR getSurface(void) const {return m_surface;}

    /// @brief Class getter for surface extent. Requires void to get member. Static-casts member width and member height into uin32_t params of VkExtent2D.
    /// @param void 
    /// @return VkExtent2D
    VkExtent2D getExtent(void) const {return {static_cast<uint32_t>(m_WIDTH), static_cast<uint32_t>(m_HEIGHT)};}

		/// @brief Creates glfw window class member. Requires void to create window.
		/// @param  void
		/// @return bool 
		bool createWindow(void);

		/// @brief Destroys glfw window class member. Requires void to destroy window.
		/// @param  void
		void destroyWindow(void);
    
		/// @brief Creates vulkan surface class member. Requires a valid instance to create surface.
		/// @param instance 
		/// @return bool 
		bool createSurface(VkInstance instance);

    /// @brief Destroys vulkan surface class member. Requires a valid instance to destroy surface.
    /// @param instance 
    void destroySurface(VkInstance instance);

	private:
		GLFWwindow *m_window;
		VkSurfaceKHR m_surface;

    std::string m_TITLE = "Venus";
    uint16_t m_WIDTH = 640;
    uint16_t m_HEIGHT = 480; 

	};
}  // namespace venus::rendersystem

#endif  // VENUS_WINDOW_HPP