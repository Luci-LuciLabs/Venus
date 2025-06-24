#include "keyboardInput.hpp"
#include "VN_logger.hpp"

// STDLIB
#include <cstdlib>

namespace venus {

	void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {  // NOLINT
		(void) mods;
		(void) scancode;
		if(key == GLFW_KEY_END && action == GLFW_PRESS) {
			VN_LOG_INFO("End key was pressed.");
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}

}  // namespace venus
