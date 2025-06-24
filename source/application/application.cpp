#include "application.hpp"

// PROJECT
#include "VN_logger.hpp"
#include "runtime.hpp"

namespace venus {

	Application::Application(const ApplicationConfigDetails &configDetails): m_details(configDetails) {
		m_runtime = std::make_unique<Runtime>(m_details);
	}

	Application::~Application() {
		m_runtime.reset();

		VN_LOG_INFO("Destroyed Venus.");
	}

	void Application::run() {
		VN_LOG_INFO("Running Venus...");
		VN_LOG_INFO(std::format("Application Identity: {} ~v{}.{}.{}", m_details.identity.name,
														m_details.identity.version.major, m_details.identity.version.minor,
														m_details.identity.version.patch));
		m_runtime->startEngine();
	}

}  // namespace venus
