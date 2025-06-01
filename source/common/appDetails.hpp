#ifndef VENUS_APP_DETAILS_HPP
#define VENUS_APP_DETAILS_HPP

namespace venus {

	struct AppDetails {
		const char *name;
		int versionMajour;
		int versionMinour;
		int versionPatch;
	};

  struct WindowDetails {
    const char* title;
    int width;
    int height;
  };

}  // namespace venus

#endif  // VENUS_APP_DETAILS_HPP