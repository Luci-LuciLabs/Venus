#ifndef VENUS_APP_DETAILS_HPP
#define VENUS_APP_DETAILS_HPP

// STDLIB
#include <cstdint>
namespace venus {

	/**
   * @brief Bit flags to set default window resolution, the default setting does not change resizeability.
   * 
   */
	enum WindowResolution_BIT : uint32_t {  // TODO: SUPPORT FULLSCREEN
		USE_480p_RES_BIT = (640 << 16) | 480,
		USE_720p_RES_BIT = (1280 << 16) | 720,
		USE_1080p_RES_BIT = (1920 << 16) | 1080,
	};

	/**
   * @brief Used to supply Venus application-layer with basic details about the client-application.
   * 
   */
	struct AppDetails {
		const char *applicationName;
		const char *windowTitle;
		WindowResolution_BIT defaultWindowResolution;
		unsigned int versionMajor;
		unsigned int versionMinor;
		unsigned int versionPatch;
	};

}  // namespace venus

#endif  // VENUS_APP_DETAILS_HPP