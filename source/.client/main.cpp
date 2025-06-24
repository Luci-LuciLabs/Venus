#include "application.hpp"

#include <iostream>
#include <memory>

auto main() -> int {
	venus::ApplicationIdentityDetails appID{.name = "Venus Client", .version = {.major = 1, .minor = 0, .patch = 0}};

	venus::WindowConfigDetails windowDetails{.title = "Venus Client",
																					 .ResolutionBit = venus::RESOLUTION_4x3_SVGA_BIT,
																					 .AspectRatioFlag = venus::ASPECT_RATIO_4_BY_3_FLAG_BIT,
																					 .WindowModeFlag = venus::WINDOW_MODE_NORMAL_FLAG_BIT};

	venus::ApplicationConfigDetails config{.identity = appID, .windowConfig = windowDetails};

	std::unique_ptr<venus::Application> VNS_APP = std::make_unique<venus::Application>(config);

	try {
		VNS_APP->run();
	} catch(const std::exception &e) {
		std::cerr << e.what() << '\n';
		return 1;
	}

	return 0;
}