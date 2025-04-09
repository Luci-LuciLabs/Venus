#include "app.hpp"

// stdlib
#include <iostream>

namespace venus_mantle{
    
    app_mantle::app_mantle(){};
    app_mantle::~app_mantle(){};

    void app_mantle::run(void){
        std::cout << "running Venus..." << '\n';
        
        while(!mantle_window.shouldClose()){
            glfwPollEvents();
        }
        
    }

} // namespace venus_mantle
