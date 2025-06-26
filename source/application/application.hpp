#ifndef VENUS_APPLICATION_HPP
#define VENUS_APPLICATION_HPP

// PROJECT
#include "venusConfigOptions.hpp"

// STDLIB
#include <memory>
namespace venus {
	class Runtime;
	/**
   * @brief A public interface into venus.
   * 
   * @class Application
   *
   * @details This object wraps the runtime class as a simple public interface 
   *          which does not expose unnecessary complexity to the client.
   *
   *          This object has an explicit contructor that requires a fully populated ApplicationConfigDetails struct.
   *          To properly start a program using Venus, you must populate the required config struct data-fields,
   *          and then call the function 'run()'.
   *
   */
	class Application {
	public:
		explicit Application(const ApplicationConfigDetails &configDetails);
		~Application();

		Application(const Application &) = delete;
		auto operator=(const Application &) -> Application & = delete;

		Application(const Application &&) = delete;
		auto operator=(const Application &&) -> Application && = delete;

		void run();

	private:
		ApplicationConfigDetails m_details;
		std::unique_ptr<Runtime> m_runtime;
	};

}  // namespace venus

#endif  // VENUS_APPLICATION_HPP