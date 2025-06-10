#ifndef VENUS_APPLICATION_HPP
#define VENUS_APPLICATION_HPP

// PROJECT
#include "appDetails.hpp"

// STDLIB
#include <memory>
namespace venus {
	class Runtime;
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
		AppDetails m_details;
		std::unique_ptr<Runtime> m_runtime;
	};

}  // namespace venus

#endif  // VENUS_APPLICATION_HPP