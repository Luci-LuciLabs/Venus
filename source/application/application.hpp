#ifndef VENUS_APPLICATION_HPP
#define VENUS_APPLICATION_HPP

// PROJECT
#include "appDetails.hpp"

// STDLIB
#include <memory>
namespace venus {
	class Window;
	class Instance;
	class Application {
	public:
		Application(const AppDetails &detailsRef);
		~Application();

		Application(const Application &) = delete;
		auto operator=(const Application &) -> Application & = delete;

		Application(const Application &&) = delete;
		auto operator=(const Application &&) -> Application && = delete;

		void run();

	private:
		AppDetails m_details;
		std::unique_ptr<Instance> m_instance;
		std::unique_ptr<Window> m_window;
	};

}  // namespace venus

#endif  // VENUS_APPLICATION_HPP