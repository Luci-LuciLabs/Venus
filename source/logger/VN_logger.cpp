#include "quill_PCH.hpp"
#include "VN_logger.hpp"

// STDLIB
#include <stdexcept>

namespace venus::log {
	namespace {  // ANONYMOUS NAMESPACE BEGIN

		// VN_LOGGER must be defined in this manner to avoid issues with lambda and function scopes.
		quill::Logger *VN_LOGGER = nullptr;  // NOLINT
		// whenever this function is called, if any logs have yet to be flushed then all logs will flush and quill will close its threads.
		void shutdown_VN_Logger() {
			quill::Backend::stop();
			VN_LOGGER = nullptr;
		}

		void init_VN_Logger() {
			// start backend thread, use backend options to adjust which cpu-thread is used.
			// do not set cpu_affinity to allow quill to choose threads itself.
			static bool isInitialized = [] {
				constexpr uint32_t BACKUP_FILE_COUNT = 10;
				constexpr auto TEN_MEGABYTES = static_cast<size_t>(10 * 1024 * 1024);

				quill::BackendOptions backend_options;
				quill::Backend::start(backend_options);

				quill::PatternFormatterOptions logPattern = quill::PatternFormatterOptions{
					"%(time) [%(thread_id)] %(short_source_location:<28) LOG_%(log_level:<9) %(logger:<20) "
					"%(message)",
					"%H:%M:%S.%Qms", quill::Timezone::GmtTime};

				// logger will always use both sinks, you do not need to call different macros/functions to get both.
				// there really is no need to fully disable terminal logging during release builds, it has almost no perfomance costs from what I can tell.
				// it also sits upon its own background thread and wont interfere with thread-performance for our core subsystems.
				auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("VN_CONSOLE_SINK");
				auto json_sink = quill::Frontend::create_or_get_sink<quill::RotatingJsonFileSink>("logs/VN_log.json", []() {
					quill::RotatingFileSinkConfig jsonConfig;
					jsonConfig.set_open_mode('a');
					jsonConfig.set_timezone(quill::Timezone::LocalTime);
					jsonConfig.set_max_backup_files(BACKUP_FILE_COUNT);
					jsonConfig.set_filename_append_option(quill::FilenameAppendOption::StartDate);
					jsonConfig.set_rotation_naming_scheme(quill::RotatingFileSinkConfig::RotationNamingScheme::Index);
					jsonConfig.set_overwrite_rolled_files(false);
					jsonConfig.set_rotation_max_file_size(TEN_MEGABYTES);
					return jsonConfig;
				}());

				VN_LOGGER = quill::Frontend::create_or_get_logger("VN_LOGGER", {std::move(console_sink), std::move(json_sink)},
																													logPattern);
				VN_LOGGER->set_immediate_flush(1);
				// ifdef-else statement is within static lambda so it does not repeatedly set log-level to the same level every call.
				// we are doing this since we have issues with using macros and compile-time-level-defines.
				// we only want errors and criticals to be logged in release builds.
				// this is a precation in that should hopefully allow it to function properly if debug/release type isnt the same as linking program if this is linked and not rebuilt.
				// the header macros SHOULD deal with that by changing to void pointer if included into a program building in release mode.
#ifdef NDEBUG
				VN_LOGGER->set_log_level(quill::LogLevel::Error);
#else
				VN_LOGGER->set_log_level(quill::LogLevel::TraceL3);
#endif
				// static lambda gets processed during first call and will always store true when initialization is true.
				// this means we can be sure the proper steps were completed no matter what portion of code calls logging macros first.
				return true;
			}();
			// used to suppress errors from -Werror since we desire this functionality.
			(void) isInitialized;

			if(VN_LOGGER == nullptr) {
				// shutdown logger will flush ALL queues before we throw.
				// we throw a full runtime_error since we REALLY desire the logging.
				// log-output on release will only report errors and crashes, we need this information to properly diagnose issues.
				// we cannot log this error.
				shutdown_VN_Logger();
				throw std::runtime_error("Logger must not be null, initialization failed.");
			}
		}

		// this function checks lambda evaluation in the init_VN_Logger() function so we dont accidentally access VN_LOGGER at nullptr or garbage address.
		// its called within the logging functions so it will initialize properly no matter the location of first call and skip init if not needed.
		// init_VN_Logger() will throw runtime error if it ever manages to become null past the lambda.
		auto get_VNlogger() -> quill::Logger * {
			init_VN_Logger();
			return quill::Frontend::get_logger("VN_LOGGER");
		}
	}  // namespace

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
