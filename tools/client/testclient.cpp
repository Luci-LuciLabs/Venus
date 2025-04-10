#include "../../sources/app/app.hpp"

// stdlib
#include <stdexcept>
#include <iostream>

// This client implementation is intended purely for testing purposes during Venus development.
// It is not representative of proper Venus use. Do not use this client as a reference to using Venus.

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