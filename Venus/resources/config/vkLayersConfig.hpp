#pragma once

#include <vector>

namespace vkLayersConfig{

  #ifdef NDEBUG
    inline const bool validationLayersEnabled = false;
  #else
    inline const bool validationLayersEnabled = true;
  #endif

  const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
  };
};
// this config file is to easily adjust vulkan validation layers without having to dig into the normal source files

// if you want to add or change validation layers please update the vector validationLayers