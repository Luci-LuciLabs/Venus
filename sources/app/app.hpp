#ifndef VENUS_APP_MANTLE_HPP
#define VENUS_APP_MANTLE_HPP

#include "../coreVulkan/window/glfwWindow.hpp"
#include "../coreVulkan/instance/instance.hpp"
#include "../coreVulkan/device/device.hpp"

// stdlib
#include <memory>
namespace venus_mantle{
    class app_mantle{
        public:
            app_mantle();
            ~app_mantle();

            app_mantle(const app_mantle &) = delete;
            app_mantle &operator=(const app_mantle &) = delete;

            void run(void);
        private:

            venus_core::instance_core mantle_instance;
            venus_core::window_core mantle_window{mantle_instance};
            venus_core::device_core mantle_device{mantle_instance, mantle_window};

    };

    app_mantle* buildApp(void){return new app_mantle();}

} // namespace venus_mantle


#endif // VENUS_APP_MANTLE_HPP