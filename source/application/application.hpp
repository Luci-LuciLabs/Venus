#ifndef VENUS_APPLICATION_HPP
#define VENUS_APPLICATION_HPP

#include "appDetails.hpp"
#include "instance.hpp"
#include "window.hpp"
#include "physicalDevice.hpp"

namespace venus {
	class Application {
	public:
		Application();
		~Application();

		Application(const Application &) = delete;
		Application &operator=(const Application &) = delete;

		void run(void);

	private:
		AppDetails app_details = {"Venus-App", 1, 0, 0};
		WindowDetails window_details = {"Venus-Window", 480, 640};

		Instance m_instance{app_details};
		Window m_window{window_details, m_instance};
		PhysicalDevice m_GPU{m_instance.getInstance(), m_window.getSurface()};
	};

}  // namespace venus

#endif  // VENUS_APPLICATION_HPP