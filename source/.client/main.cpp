#include "application.hpp"

#include <iostream>
#include <memory>

auto main() -> int {
	venus::AppDetails details = {.applicationName = "V_client",
															 .windowTitle = "Venus Client",
															 .defaultWindowResolution = venus::USE_720p_RES_BIT,
															 .versionMajor = 1,
															 .versionMinor = 0,
															 .versionPatch = 0};

	std::unique_ptr<venus::Application> VNS_APP = std::make_unique<venus::Application>(details);

	try {
		VNS_APP->run();
	} catch(const std::exception &e) {
		std::cerr << e.what() << '\n';
		return 1;
	}

	return 0;
}