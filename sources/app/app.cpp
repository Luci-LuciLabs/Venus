#include "app.hpp"

// stdlib
#include <iostream>

namespace venus_mantle{
    
    app_mantle::app_mantle(){};
    app_mantle::~app_mantle(){
        std::cout << "app destroyed." << '\n';
    };

    void app_mantle::run(void){
        std::cout << "running Venus..." << '\n';
        
        while(!mantle_window.shouldClose()){
            glfwPollEvents();
        }
        
        std::cout << "stopping Venus..." << '\n';
    }

} // namespace venus_mantle
