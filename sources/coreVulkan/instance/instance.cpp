#include "instance.hpp"
#include "instanceConfig.hpp"
#include "validationLayers.hpp"
#include "glfwExtensions.hpp"

// stdlib
#include <stdexcept>
#include <vector>
#include <iostream>

namespace venus_core{

    instance_core::instance_core(){

        if(volkInitialize() != VK_SUCCESS){
            throw std::runtime_error("FAILED TO INITIALIZE VOLK LOADER!!!");
        }

        if(ENABLE_VALIDATION_LAYERS && !instanceLayers::checkValidationLayerSupport()){
            throw std::runtime_error("VALIDATION LAYERS REQUESTED BUT NOT AVAILABLE!!!");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Venus";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Venus";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_4;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto instanceExtensions = instanceExtensions::requiredInstanceExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
        createInfo.ppEnabledExtensionNames = instanceExtensions.data();

        if(ENABLE_VALIDATION_LAYERS){
            createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
        }else{
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }

        if(vkCreateInstance(&createInfo, nullptr, &member_Instance) != VK_SUCCESS){
            throw std::runtime_error("FAILED TO CREATE VULKAN INSTANCE!!!");
        }

        volkLoadInstance(member_Instance);
        
        std::cout << "instance created." << '\n';

        instanceLayers::setupDebugMessenger(member_Instance, debugMessenger);
    };

    instance_core::~instance_core(){

        if(ENABLE_VALIDATION_LAYERS){
            instanceLayers::destroyDebugUtilsMessengerEXT(member_Instance, debugMessenger, nullptr);
        }

        if(member_Instance != VK_NULL_HANDLE){
            vkDestroyInstance(member_Instance, nullptr);
            member_Instance = VK_NULL_HANDLE;
        }

        std::cout << "instance destroyed." << '\n';
    };

} // namespace venus_core
