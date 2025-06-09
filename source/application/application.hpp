#ifndef VENUS_APPLICATION_HPP
#define VENUS_APPLICATION_HPP

// PROJECT
#include "appDetails.hpp"

// STDLIB
#include <memory>
namespace venus {
	class RuntimeBootstrapper;
	class VulkanCore;
	class Window;
	class Application {
	public:
		explicit Application(const AppDetails &detailsRef);
		~Application();

		Application(const Application &) = delete;
		auto operator=(const Application &) -> Application & = delete;

		Application(const Application &&) = delete;
		auto operator=(const Application &&) -> Application && = delete;

		void run();

	private:
		std::unique_ptr<RuntimeBootstrapper> BOOT_STRAPPER;
		AppDetails m_details;
		std::unique_ptr<VulkanCore> m_vulkanCore;
		std::unique_ptr<Window> m_window;
	};

}  // namespace venus

#endif  // VENUS_APPLICATION_HPP