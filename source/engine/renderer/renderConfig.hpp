#ifndef VENUS_RENDERER_CONFIG_HPP
#define VENUS_RENDERER_CONFIG_HPP

namespace venus {

	// Maximum number of in flight frames, used to control amount of command buffers, semaphores, fences, and swapchain image count.
	static constexpr unsigned int MAX_FRAMES_IN_FLIGHT = 4;

}  // namespace venus

#endif  // VENUS_RENDERER_CONFIG_HPP