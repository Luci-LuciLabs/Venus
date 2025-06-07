#include "application.hpp"
// PROJECT
#include "VN_logger.hpp"
#include "appDetails.hpp"
#include "instance.hpp"
#include "systemProperties.hpp"
#include "window.hpp"

// STDLIB
#include <memory>
#include <string>
#include <string_view>

namespace venus {

	Application::Application(const AppDetails &detailsRef): m_details(detailsRef) {
		const int GIGABYTE = 1024 * 1024 * 1024;
		const SystemProperties properties;
		auto info = properties.getInfo();
		const std::string memoryMessage =
			"Total Ram: " + std::to_string((info.totalram * info.mem_unit) / GIGABYTE) + "gb" + '\n';
		const std::string threadMessage = "Thread count: " + std::to_string(properties.getThreadCount()) + '\n';

		VN_LOG_INFO(memoryMessage);
		VN_LOG_INFO(threadMessage);

		m_window = std::make_unique<Window>(m_details);
		m_instance = std::make_unique<Instance>(m_details);
	}

	Application::~Application() {
		VN_LOG_INFO("Destroying application.");
		m_instance.reset();
		m_window.reset();
		VN_LOG_INFO("Destroyed Venus.");
	}

	void Application::run() {
		VN_LOG_INFO("Running Venus...");
		const std::string name = m_details.applicationName;
		const std::string appInfo = "Running Application... " + name + '\n';
		VN_LOG_INFO(appInfo);

		while(!m_window->shouldClose()) {
			glfwPollEvents();
		}
	}

}  // namespace venus
