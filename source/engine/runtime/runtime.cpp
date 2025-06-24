#include "runtime.hpp"
#include "VN_logger.hpp"
#include "instance.hpp"
#include "renderer.hpp"
#include "window.hpp"

// THIRD PARTY
#include "volk.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace venus {
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
   * @brief A runtime bootstrapper object.
   *
   * @class RuntimeBootstrapper0
   *
   * @details This object manages initialization of dependency libraries and 
   *          the lifetime of constant objects, such as the vulkan instance. 
   *          
   *          This object must remain on the main thread, since it controls glfw initialization.
   *          This object must be the first runtime object created and initialized.
   *
   *          This object is not copyable. This object is not moveable.
   * 
   */
	class RuntimeBootstrapper {
	public:
		RuntimeBootstrapper(const RuntimeBootstrapper &) = delete;
		auto operator=(const RuntimeBootstrapper &) -> RuntimeBootstrapper & = delete;
		RuntimeBootstrapper(const RuntimeBootstrapper &&) = delete;
		auto operator=(const RuntimeBootstrapper &&) -> RuntimeBootstrapper && = delete;

		RuntimeBootstrapper(const ApplicationIdentityDetails &appID) {
			// error callback must be set before initializing glfw
			// initialize glfw first just in case it affects definitions loaded for vulkan
			glfwSetErrorCallback(glfwErrorCallbackFunc);
			if(glfwInit() != GLFW_TRUE) {
				VN_LOG_CRITICAL("Failed to initialize glfw.");
				glfwSetErrorCallback(nullptr);
				throw std::runtime_error("Failed to initialize glfw.");
			}

			// volk must be intialized before any further vulkan can be used.
			if(volkInitialize() != VK_SUCCESS) {
				VN_LOG_CRITICAL("Failed to initialize volk-meta-loader.");
				throw std::runtime_error("Failed to initialize volk-meta-loader.");
			}

			// we check vulkan system version before instance creation since
			// we make use of 1.4 and do not support older versions.
			uint32_t vk_versionFound = VK_API_VERSION_1_0;
			vkEnumerateInstanceVersion(&vk_versionFound);

			if(vk_versionFound < VK_API_VERSION_1_4) {
				VN_LOG_CRITICAL("Unable to find Vulkan version 1.4, check system version.");
				throw std::runtime_error("Unable to find Vulkan version 1.4, check system version.");
			}

			vulkan_runtime_instance = std::make_unique<Instance>(appID);

			VN_LOG_INFO("Runtime Bootstrapper has finished initializing.");
		}

		~RuntimeBootstrapper() {
			vulkan_runtime_instance.reset();
			volkFinalize();
			glfwTerminate();
			glfwSetErrorCallback(nullptr);
			VN_LOG_INFO("Runtime Bootstrapper has been terminated.");
		}

		// Setting this function is necessary to receive glfw error output.
		static void glfwErrorCallbackFunc(int error, const char *desc) {
			VN_LOG_ERROR(std::format("GLFW ERROR:{} : {}", error, desc));
		}

	private:
		// The vulkan instance is accessible anywhere throughout venus with 'volkGetLoadedInstance()'.
		// We manage its lifetime in this class to ensure it remains valid for all of runtime.
		std::unique_ptr<Instance> vulkan_runtime_instance;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Runtime::Runtime(const ApplicationConfigDetails &configDetails): m_details(configDetails) {
		m_bootStrapper = std::make_unique<RuntimeBootstrapper>(configDetails.identity);
		m_window = std::make_shared<Window>(m_details.windowConfig);
		m_renderer = std::make_unique<Renderer>(m_window);
		VN_LOG_INFO("Venus Runtime has been created.");
	}

	void Runtime::startEngine() {
		while(!m_window->shouldClose()) {
			glfwPollEvents();  // polls for window and input events handled by glfw.
			m_renderer->draw();
		}
	}

	Runtime::~Runtime() {
		m_renderer.reset();
		m_window.reset();
		m_bootStrapper.reset();
		VN_LOG_INFO("Venus Runtime has been destroyed.");
	}

}  // namespace venus
