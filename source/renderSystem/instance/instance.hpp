#ifndef VENUS_INSTANCE_HPP
#define VENUS_INSTANCE_HPP

#include "volk.h"
#include "common.hpp"

// STDLIB
#include <vector>

namespace venus::rendersystem {

/// @brief Controls whether or not vulkan validation layers are used.
#ifdef NDEBUG
		const bool ENABLE_VALIDATION_LAYERS = false;
#else
		const bool ENABLE_VALIDATION_LAYERS = true;
#endif

	/// @brief Controls the vulkan instance, intitialization of volk and glfw, and the validation layer debug messenger.
  ///
  /// @details
  /// This class should always be the first class created and called within the renderSystem.
  /// Instance creation will also handle initialization of glfw and volk.
  /// If 'ENABLE_VALIDATION_LAYERS' is true then 'createInstance()' will also create a validation layer debug messenger.
  ///
  /// This class should always be the last class created and called within the renderSystem.
  /// Instance destruction will also deinitialize glfw and volk. 
  /// If the validation layer debug messenger has been created Instance destruction will also destroy the messenger.
  ///
  /// This class is entirely self-contained and is NOT to ever require outside dependencies.
	class instance {
	public:
		/// @brief This class requires no constructor members.
		instance();
		~instance();

		/// @brief This class should NEVER be copied.
		instance(const instance &) = delete;
		/// @brief This class should NEVER be copied. 
		instance &operator=(const instance &) = delete;

		/// @brief Gets class member vulkan instance.
		/// @param  void
		/// @return VkInstance
		VkInstance getInstance(void) const { return m_instance; }

    /// @brief creates vulkan instance.
    /// @param  void
    void createInstance(void);
    /// @brief destroys vulkan instance.
    /// @param  void
    void destroyInstance(void);


	private:
		/// @brief Holds the vulkan instance itself. Default as VK_NULL_HANDLE.
		VkInstance m_instance = VK_NULL_HANDLE;
		/// @brief Holds the vulkan validation messenger. Default as VK_NULL_HANDLE.
		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

    appDetails details;

		/// @brief Holds all chosen vulkan validation layers. Must contain "VK_LAYER_KHRONOS_validation".
		std::vector<const char *> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

		/// @brief Gets all required extensions for instance creation.
		/// @param void 
		/// @return vector<const char*>
		std::vector<const char *> getRequiredExtentions(void);
		/// @brief Checks if all requested vulkan validation layers are supported on the host system.
		/// @param  void
		/// @return bool 
		bool checkValidationLayerSupport(void);
    /// @brief Handles configuration and creation of vulkan validation layer debug messenger.
    /// @param instance 
    /// @param debugMessenger 
    void setupDebugMessenger(VkInstance &instance, VkDebugUtilsMessengerEXT &debugMessenger);
  };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // clang-format off

		/// @brief Sets the validation layer debugCallback functionality.
		/// @param messageSeverity 
		/// @param messageType 
		/// @param pCallbackData 
		/// @param pUserData 
		/// @return VKAPI_ATTR VkBool32 VKAPI_CALL
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
																											 VkDebugUtilsMessageTypeFlagsEXT messageType,
																											 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
																											 void *pUserData);


		/// @brief Creates validation layer debug messenger.
		/// @param instance 
		/// @param pcreateInfo 
		/// @param pAllocator 
		/// @param pDebugMessenger 
		/// @return VkResult
		VkResult createDebugUtilsMessengerEXT(VkInstance &instance, const VkDebugUtilsMessengerCreateInfoEXT *pcreateInfo,
																					const VkAllocationCallbacks *pAllocator,
																					VkDebugUtilsMessengerEXT *pDebugMessenger);


		/// @brief Destroys validaion layer debug messenger.
		/// @param instance 
		/// @param debugMessenger 
		/// @param pAllocator 
		void destroyDebugUtilsMessengerEXT(VkInstance &instance, VkDebugUtilsMessengerEXT debugMessenger,
																			 const VkAllocationCallbacks *pAllocator);

    // clang-format on
}  // namespace venus::rendersystem
#endif  // VENUS_INSTANCE_HPP