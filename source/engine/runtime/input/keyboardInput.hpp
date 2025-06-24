#ifndef VENUS_KEYBOARD_INPUT_HPP
#define VENUS_KEYBOARD_INPUT_HPP

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace venus {

	void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

};

#endif  // VENUS_KEYBOARD_INPUT_HPP
