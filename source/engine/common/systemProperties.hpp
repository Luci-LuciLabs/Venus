#ifndef VENUS_SYSTEM_PROPERTIES_HPP
#define VENUS_SYSTEM_PROPERTIES_HPP

#include <cstdint>
#include <sys/sysinfo.h>
#include <thread>

namespace venus {

	/**
   * @brief Used to query and store details about the user-system, currently used exclusively for exploring things during development.
   * 
   */
	struct SystemProperties {
	private:
		struct sysinfo info;
		uint32_t threadCount;

		static auto storeSysInfo() -> struct sysinfo {
			struct sysinfo info {};
			sysinfo(&info);
			return info;
		}

		static auto
		storeThreadCount() -> uint32_t {
			return std::thread::hardware_concurrency();
		}

	public:
		SystemProperties(): info(storeSysInfo()), threadCount(storeThreadCount()){};

		[[nodiscard]] auto getThreadCount() const -> uint32_t { return threadCount; }

		[[nodiscard]] auto getInfo() const -> struct sysinfo {
			return info;
		}
	};

};  // namespace venus

#endif  // VENUS_SYSTEM_PROPERTIES_HPP