#ifndef VENUS_LOGGER_SYSTEM_HPP
#define VENUS_LOGGER_SYSTEM_HPP

// STDLIB
#include <source_location>
#include <string_view>

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

// Clang-tidy complains that these are "function like macros", and they are.
// It's done this way to avoid exposing ANY of Quill (logging library).
// Quill is header only and would require more work to upkeep if we fully integrated it.
// We opt to precompile necessary Quill headers and wrap the library with these macros and functions.
// Macros are ALWAYS the preferred way to use this logger. Functions are an option, but not encouraged.
// DO NOT CHANGE MACRO NAMES, CHANGES TO THE LOGGER MUST RETAIN PREVIOUS NAMES AND ACCOMODATE PREVIOUS USAGE WITHIN CODEBASE.

// NOLINTBEGIN
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
// NOLINTEND
#endif  // VENUS_LOGGER_SYSTEM_HPP