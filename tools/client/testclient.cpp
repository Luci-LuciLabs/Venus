#include "../../sources/app/app.hpp"

// stdlib
#include <stdexcept>
#include <iostream>

int main(void){
    auto app = venus_mantle::buildApp();

    try{
        app->run();
    }catch(const std::exception &e){
        std::cerr << e.what() << '\n';
        delete app;
        return EXIT_FAILURE;
    }

    delete app;
    return EXIT_SUCCESS;
}