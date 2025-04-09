#ifndef VENUS_APP_MANTLE_HPP
#define VENUS_APP_MANTLE_HPP

#include "../coreVulkan/window/glfwWindow.hpp"
#include "../coreVulkan/instance/instance.hpp"

// stdlib

namespace venus_mantle{
    class app_mantle{
        public:
            app_mantle();
            ~app_mantle();

            app_mantle(const app_mantle &) = delete;
            app_mantle &operator=(const app_mantle &) = delete;

            void run(void);
        private:

            venus_core::window_core mantle_window;
            venus_core::instance_core mantle_instance;
    };

    app_mantle* buildApp(void){return new app_mantle();}

} // namespace venus_mantle


#endif // VENUS_APP_MANTLE_HPP