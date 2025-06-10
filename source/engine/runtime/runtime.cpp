#include "runtime.hpp"
#include "VN_logger.hpp"
#include "vulkanCore.hpp"
#include "window.hpp"

// THIRD PARTY
#include "volk.h"

namespace venus {

	class RuntimeBootstrapper {
	public:
		// This class should NEVER be moved nor copied.
		// This class should only ever be called once at the beginning of the application,
		// so it may load dependencies required for further functionality.
		// This class must remain on the main thread as it handles the initialization of GLFW.
		// GLFW must be on the main thread along with any window and input implementations derived from GLFW.
		// We check the version of vulkan on the system to decide if its aqequate before attempting instance creation.

		RuntimeBootstrapper(const RuntimeBootstrapper &) = delete;
		auto operator=(const RuntimeBootstrapper &) -> RuntimeBootstrapper & = delete;
		RuntimeBootstrapper(const RuntimeBootstrapper &&) = delete;
		auto operator=(const RuntimeBootstrapper &&) -> RuntimeBootstrapper && = delete;

		RuntimeBootstrapper() {
			glfwSetErrorCallback(glfwErrorCallbackFunc);
			if(glfwInit() != GLFW_TRUE) {
				VN_LOG_CRITICAL("Failed to initialize glfw.");
				glfwSetErrorCallback(nullptr);
				throw std::runtime_error("Failed to initialize glfw.");
			}
			if(volkInitialize() != VK_SUCCESS) {
				VN_LOG_CRITICAL("Failed to initialize volk-meta-loader.");
				throw std::runtime_error("Failed to initialize volk-meta-loader.");
			}

			uint32_t vk_versionFound = VK_API_VERSION_1_0;
			vkEnumerateInstanceVersion(&vk_versionFound);
			if(vk_versionFound < VK_API_VERSION_1_4) {
				VN_LOG_CRITICAL("Unable to find Vulkan version 1.4, check system version.");
				throw std::runtime_error("Unable to find Vulkan version 1.4, check system version.");
			}

			VN_LOG_INFO("Runtime Bootstrapper has finished initializing.");
		}

		~RuntimeBootstrapper() {
			volkFinalize();
			glfwTerminate();
			glfwSetErrorCallback(nullptr);
			VN_LOG_INFO("Runtime Bootstrapper has been terminated.");
		}

		static void glfwErrorCallbackFunc(int error, const char *desc) {
			(void) error;
			VN_LOG_ERROR(std::format("GLFW ERROR: {}", desc));
		}
	};

	Runtime::Runtime(const AppDetails &detailsRef): m_details(detailsRef) {
		m_bootstrapper = std::make_unique<RuntimeBootstrapper>();

		m_window = std::make_unique<Window>(m_details);
		m_vulkanCore = std::make_unique<VulkanCore>(m_window);
	}

	void Runtime::startEngine() {
		while(!m_window->shouldClose()) {
			glfwPollEvents();
		}
	}

	Runtime::~Runtime() {
		m_window.reset();
		m_vulkanCore.reset();
		m_bootstrapper.reset();
	}

}  // namespace venus
