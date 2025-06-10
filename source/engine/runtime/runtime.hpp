#ifndef VENUS_ENGINE_RUNTIME_HPP
#define VENUS_ENGINE_RUNTIME_HPP

// PROJECT
#include "appDetails.hpp"

// STDLIB
#include <memory>

namespace venus {
	class RuntimeBootstrapper;
	class Window;
	class VulkanCore;
	class Runtime {
	public:
		explicit Runtime(const AppDetails &detailsRef);
		~Runtime();

		Runtime(const Runtime &) = delete;
		auto operator=(const Runtime &) -> Runtime & = delete;
		Runtime(const Runtime &&) = delete;
		auto operator=(const Runtime &&) -> Runtime & = delete;

		void startEngine();

	private:
		AppDetails m_details;
		std::unique_ptr<RuntimeBootstrapper> m_bootstrapper;
		std::unique_ptr<Window> m_window;
		std::unique_ptr<VulkanCore> m_vulkanCore;
	};

}  // namespace venus

#endif  // VENUS_ENGINE_RUNTIME_HPP