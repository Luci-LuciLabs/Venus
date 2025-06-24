#ifndef VENUS_CONFIG_OPTIONS_HPP
#define VENUS_CONFIG_OPTIONS_HPP

// STDLIB
#include <cstdint>
namespace venus {
	using VNS_8b_FLAG = uint8_t;
	using VNS_16b_FLAG = uint16_t;
	using VNS_32b_FLAG = uint32_t;

	using VNS_RESOLUTION_BIT = VNS_32b_FLAG;

	static constexpr uint32_t BIT_SHIFT_16 = 16;

	/**
   * @brief Native Window Resolution bit flag.
   *
   * @details This bitmask has a void value since native resolutions are indeterminate and are required to be polled from the monitor.
   *          Therefore we use it as a bit-flag instead of a proper bitmask. Using this flag in place of fully valued VNS_RESOLUTION_BIT types
   *          will tell Venus to start the application using the native resolution of the monitors current video mode.
   *
   *          It's possible that using native resolution may allow the application to use lower resolutions than already supported,
   *          however this has not been tested and relies entirely upon the video mode of the primary monitor. Attempting this will
   *          not work when using "WINDOW_MODE_NORMAL_FLAG_BIT" during window configuration.
   */
	static const VNS_RESOLUTION_BIT RESOLUTION_NATIVE_BIT = 0;

	/**
   * @brief 4x3 Window Resolution bitmasks.
   *
   * @details The minimum supported resolution is 480p. All bitmasks in this enum are in 4x3 format and have standardized values.
   *          If you need to use an unsupported 4x3 resolution you should use the "makeCustomResolution_BIT" function, it may be
   *          used to directly populate "WindowConfigDetails::defaultResolutionBit" field.
   */
	enum Resolution_4x3_BIT : VNS_RESOLUTION_BIT {
		RESOLUTION_4x3_VGA_BIT = (640 << BIT_SHIFT_16) | 480,
		RESOLUTION_4x3_SVGA_BIT = (800 << BIT_SHIFT_16) | 600,
		RESOLUTION_4x3_XGA_BIT = (1024 << BIT_SHIFT_16) | 768,
		RESOLUTION_4x3_XGA_PLUS_BIT = (1152 << BIT_SHIFT_16) | 864,
		RESOLUTION_4x3_QXGA_BIT = (2048 << BIT_SHIFT_16) | 1536,
		RESOLUTION_4x3_QUXGA_BIT = (3200 << BIT_SHIFT_16) | 2400,
		RESOLUTION_4x3_HXGA_BIT = (4096 << BIT_SHIFT_16) | 3072
	};

	/**
   * @brief 16x9 Window Resolution bitmasks.
   *
   * @details The minimum supported resolution is 480p. All bitmasks in this enum are in 16x9 format and have standardized values.
   *          If you need to use an unsupported 16x9 resolution you should use the "makeCustomResolution_BIT" function, it may be
   *          used to directly populate "WindowConfigDetails::defaultResolutionBit" field.
   */
	enum Resolution_16x9_BIT : VNS_RESOLUTION_BIT {
		RESOLUTION_16x9_FWVGA_BIT = (854 << BIT_SHIFT_16) | 480,
		RESOLUTION_16x9_QHD_QUARTER_BIT = (960 << BIT_SHIFT_16) | 540,
		RESOLUTION_16x9_WSVGA_BIT = (1024 << BIT_SHIFT_16) | 576,
		RESOLUTION_16x9_HD_BIT = (1280 << BIT_SHIFT_16) | 720,
		RESOLUTION_16x9_FWXGA_BIT = (1366 << BIT_SHIFT_16) | 768,
		RESOLUTION_16x9_HD_PLUS_BIT = (1600 << BIT_SHIFT_16) | 900,
		RESOLUTION_16x9_FHD_BIT = (1920 << BIT_SHIFT_16) | 1080,
		RESOLUTION_16x9_QWXGA_BIT = (2048 << BIT_SHIFT_16) | 1152,
		RESOLUTION_16x9_QHD_QUAD_BIT = (2560 << BIT_SHIFT_16) | 1440,
		RESOLUTION_16x9_WQXGA_PLUS_BIT = (3200 << BIT_SHIFT_16) | 1800,
		RESOLUTION_16x9_UHD_BIT = (3840 << BIT_SHIFT_16) | 2160,
		RESOLUTION_16x9_UHD_PLUS_BIT = (5120 << BIT_SHIFT_16) | 2880,
		RESOLUTION_16x9_FUHD_BIT = (7680 << BIT_SHIFT_16) | 4320
	};

	/**
   * @brief Creates a custom resolution bitmask.
   *
   * @details Allows the client executable to create a custom resolution bitmask to pass into the 'WindowConfigDetails::defaultResolutionBit' field. 
   *          This bitmask is in the same format and is processed the same as the standard 'WindowResolution_BIT' members.
   *          It should be kept in mind that the lowest supported resolution is 480p and any custom resolution lower than that will automatically be pushed to 480p.
   * 
   * @param (uint16_t) width 
   * @param (uint16_t) height 
   * @return VNS_RESOLUTION_BIT
   */
	inline auto makeCustomResolution_BIT(uint16_t width, uint16_t height) -> VNS_RESOLUTION_BIT {
		uint32_t CUSTOM_RES_BIT = (width << BIT_SHIFT_16) | height;
		return static_cast<VNS_RESOLUTION_BIT>(CUSTOM_RES_BIT);
	}

	enum AspectRatio : VNS_8b_FLAG { ASPECT_RATIO_16_BY_9_FLAG_BIT = 1, ASPECT_RATIO_4_BY_3_FLAG_BIT = 2 };

	/**
   * @brief Windowing Mode bit-flags.
   *
   * @details Sets the default windowing mode, windowing modes are currently configured for all of runtime.
   */
	enum WindowModeFlags : uint8_t {
		WINDOW_MODE_NORMAL_FLAG_BIT = 1 << 0,
		WINDOW_MODE_FULLSCREEN_FLAG_BIT = 1 << 1,
		WINDOW_MODE_BORDERLESS_FLAG_BIT = 1 << 2
	};

	struct ApplicationVersion {
		unsigned int major;
		unsigned int minor;
		unsigned int patch;
	};

	struct WindowConfigDetails {
		const char *title;
		VNS_RESOLUTION_BIT ResolutionBit;
		AspectRatio AspectRatioFlag;
		WindowModeFlags WindowModeFlag;
	};

	struct ApplicationIdentityDetails {
		const char *name;
		ApplicationVersion version;
	};

	/**
   * @brief Configures how exactly Venus should build your app.
   */
	struct ApplicationConfigDetails {
		ApplicationIdentityDetails identity;
		WindowConfigDetails windowConfig;
	};

}  // namespace venus

#endif  // VENUS_CONFIG_OPTIONS_HPP