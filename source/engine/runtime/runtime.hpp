#ifndef VENUS_ENGINE_RUNTIME_HPP
#define VENUS_ENGINE_RUNTIME_HPP

// PROJECT
#include "venusConfigOptions.hpp"

// STDLIB
#include <memory>

namespace venus {
	class RuntimeBootstrapper;
	class Window;
	class Renderer;

	/**
   * @brief A runtime manager object.
   * 
   * @class Runtime
   *
   * @details This object manages the runtime loop and the necessary components for loop steps.
   *
   *          This object cannot be copied. This object cannot be moved.
   */
	class Runtime {
	public:
		explicit Runtime(const ApplicationConfigDetails &configDetails);
		~Runtime();

		Runtime(const Runtime &) = delete;
		auto operator=(const Runtime &) -> Runtime & = delete;
		Runtime(const Runtime &&) = delete;
		auto operator=(const Runtime &&) -> Runtime & = delete;

		void startEngine();

	private:
		ApplicationConfigDetails m_details;
		std::unique_ptr<RuntimeBootstrapper> m_bootStrapper;
		std::shared_ptr<Window> m_window;  // Window is needed by Renderer class.

		std::unique_ptr<Renderer> m_renderer;
	};

}  // namespace venus

#endif  // VENUS_ENGINE_RUNTIME_HPP