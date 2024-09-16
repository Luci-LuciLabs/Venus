#include "../app/appCore.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main(){
  CORE::appCORE app;

  try{
    app.run();
  } catch(const std::exception &except){
      std::cerr << except.what();
      return EXIT_FAILURE;
  }
return EXIT_SUCCESS;
}