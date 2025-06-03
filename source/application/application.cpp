#include "application.hpp"
#include "VN_logger.hpp"

#include <thread>

namespace venus {

	Application::Application() {
		unsigned int threadCount = std::thread::hardware_concurrency();
		std::string msg = "Thread count: " + std::to_string(threadCount) + '\n';
		VN_LOG_INFO(msg);
	}
	Application::~Application() {}

	void Application::run(void) {
		VN_LOG_INFO("Running Venus...");
		while(!m_window.shouldClose()) {
			glfwPollEvents();
		}
	}

}  // namespace venus
