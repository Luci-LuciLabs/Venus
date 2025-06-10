#include "application.hpp"

// PROJECT
#include "VN_logger.hpp"
#include "runtime.hpp"

namespace venus {

	Application::Application(const AppDetails &detailsRef): m_details(detailsRef) {
		m_runtime = std::make_unique<Runtime>(m_details);
	}

	Application::~Application() {
		VN_LOG_INFO("Destroying application.");

		m_runtime.reset();

		VN_LOG_INFO("Destroyed Venus.");
	}

	void Application::run() {
		VN_LOG_INFO("Running Venus...");
		m_runtime->startEngine();
	}

}  // namespace venus
