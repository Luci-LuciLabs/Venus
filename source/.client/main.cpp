#include "application.hpp"

#include <stdexcept>
#include <iostream>

int main(void) {
	venus::Application app;

	try {
		app.run();
	} catch(const std::exception &e) {
		std::cerr << e.what() << '\n';
	}

	return 0;
}