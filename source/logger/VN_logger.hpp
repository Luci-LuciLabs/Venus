#ifndef VENUS_LOGGER_SYSTEM_HPP
#define VENUS_LOGGER_SYSTEM_HPP

// STDLIB
#include <string_view>
#include <source_location>

// you should always favour the VN_LOG_XXXX macros over because the contents of this namespace
// are not intended to be used directly but could function if passed the correct parameters.
// USAGE EX:
// log_info("message", std::source_location::current());
namespace venus::log {
  void log_trace(std::string_view msg, const std::source_location &location);
	void log_info(std::string_view msg, const std::source_location &location);
	void log_debug(std::string_view msg, const std::source_location &location);
	void log_warn(std::string_view msg, const std::source_location &location);
	void log_error(std::string_view msg, const std::source_location &location);
	void log_critical(std::string_view msg, const std::source_location &location);
}  // namespace venus::log

#ifdef NDEBUG
	#define VN_LOG_TRACE(msg) (void) 0
  #define VN_LOG_INFO(msg) (void) 0
	#define VN_LOG_DEBUG(msg) (void) 0
	#define VN_LOG_WARN(msg) (void) 0
#else
  #define VN_LOG_TRACE(msg) venus::log::log_trace(msg, std::source_location::current())
	#define VN_LOG_INFO(msg) venus::log::log_info(msg, std::source_location::current())
	#define VN_LOG_DEBUG(msg) venus::log::log_debug(msg, std::source_location::current())
	#define VN_LOG_WARN(msg) venus::log::log_warn(msg, std::source_location::current())
#endif

#define VN_LOG_ERROR(msg) venus::log::log_error(msg, std::source_location::current())
#define VN_LOG_CRITICAL(msg) venus::log::log_critical(msg, std::source_location::current())

#endif  // VENUS_LOGGER_SYSTEM_HPP