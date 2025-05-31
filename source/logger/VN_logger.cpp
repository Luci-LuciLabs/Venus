#include "quill_PCH.hpp"
#include "VN_logger.hpp"

#include <stdexcept>

namespace venus::log {
  // VN_LOGGER must be defined in this manner to avoid issues with lambda and function scopes.
  static quill::Logger *VN_LOGGER = nullptr;

  // whenever this function is called, if any logs have yet to be flushed then all logs will flush and quill will close its threads.
  void shutdown_VN_Logger(void){
    quill::Backend::stop();
    VN_LOGGER = nullptr;
  }

	void init_VN_Logger(void) {
		// start backend thread, use backend options to adjust which cpu-thread is used.
		// do not set cpu_affinity to allow quill to choose threads itself.
		static bool isInitialized = [] {
			quill::BackendOptions backend_options;
			quill::Backend::start(backend_options);

			const quill::PatternFormatterOptions pattern_options{
				"%(time) thread:[%(thread_id)] %(short_source_location:<28) LOG_%(log_level:<9) %(logger:<12) \n MESSAGE: "
				"%(message) ",
				"%H:%M:%S.%Qms", quill::Timezone::GmtTime};

			// logger will always use this sink for terminal output.
			auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("VN_CONSOLE_SINK");
			VN_LOGGER = quill::Frontend::create_or_get_logger("VN_LOGGER", console_sink, pattern_options);

			// we are doing this since we have issues with using macros and compile-time-level-defines.
			// we only want errors and criticals to be logged in release builds.
			// this is a precation in that should hopefully allow it to function properly if debug/release type isnt the same as linking program if this is linked and not rebuilt.
			// the header macros SHOULD deal with that by changing to void pointer if included into a program building in release mode.
#ifdef NDEBUG
			VN_LOGGER->set_log_level(quill::LogLevel::Error);
#else
			VN_LOGGER->set_log_level(quill::LogLevel::TraceL3);
#endif

			return true;
		}();
		(void) isInitialized;

		if(!VN_LOGGER) {
      shutdown_VN_Logger();
			throw std::runtime_error("Logger must not be null, initialization failed.");
		}
	}

  // this function checks lambda evaluation in the init_VN_Logger() function so we dont accidentally access VN_LOGGER at nullptr or garbage address.
  // its called within the logging functions so it will initialize properly no matter the location of first call and skip init if not needed.
  // init_VN_Logger() will throw runtime error if it ever manages to become null past the lambda.
	quill::Logger *get_VNlogger(void) {
		init_VN_Logger();
		return quill::Frontend::get_logger("VN_LOGGER");
	}


	void log_trace(std::string_view msg, const std::source_location &location) {
		QUILL_LOG_RUNTIME_METADATA(get_VNlogger(), quill::LogLevel::TraceL3, location.file_name(), location.line(),
															 location.function_name(), "{}", msg);
	}
	void log_info(std::string_view msg, const std::source_location &location) {
		QUILL_LOG_RUNTIME_METADATA(get_VNlogger(), quill::LogLevel::Info, location.file_name(), location.line(),
															 location.function_name(), "{}", msg);
	}
	void log_debug(std::string_view msg, const std::source_location &location) {
		QUILL_LOG_RUNTIME_METADATA(get_VNlogger(), quill::LogLevel::Debug, location.file_name(), location.line(),
															 location.function_name(), "{}", msg);
	}
	void log_warn(std::string_view msg, const std::source_location &location) {
		QUILL_LOG_RUNTIME_METADATA(get_VNlogger(), quill::LogLevel::Warning, location.file_name(), location.line(),
															 location.function_name(), "{}", msg);
	}
	void log_error(std::string_view msg, const std::source_location &location) {
		QUILL_LOG_RUNTIME_METADATA(get_VNlogger(), quill::LogLevel::Error, location.file_name(), location.line(),
															 location.function_name(), "{}", msg);
	}
	void log_critical(std::string_view msg, const std::source_location &location) {
		QUILL_LOG_RUNTIME_METADATA(get_VNlogger(), quill::LogLevel::Critical, location.file_name(), location.line(),
															 location.function_name(), "{}", msg);
	}

}  // namespace venus::log
