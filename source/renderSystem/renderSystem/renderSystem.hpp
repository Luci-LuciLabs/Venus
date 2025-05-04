#ifndef VENUS_RENDERSYSTEM_HPP
#define VENUS_RENDERSYSTEM_HPP

#include "instance.hpp"
#include "window.hpp"
#include "physicalDevice.hpp"
#include "logicalDevice.hpp"
#include "swapchain.hpp"
  
// STDLIB
#include <memory>

namespace venus::rendersystem{
  class renderSystem{
    public:
      renderSystem();
      ~renderSystem();
      
      void runLoop(void);
      void initRenderSystem(void);
      void killRenderSystem(void);

    private:
      std::unique_ptr<instance> renderInstance = std::make_unique<instance>();
      std::unique_ptr<window> renderWindow = std::make_unique<window>();
      std::unique_ptr<physicalDevice> renderPhysicalDevice = std::make_unique<physicalDevice>();
      std::unique_ptr<logicalDevice> renderLogicalDevice = std::make_unique<logicalDevice>();
      std::unique_ptr<swapchain> renderSwapchain = std::make_unique<swapchain>();

  };

} // namespace venus::rendersystem

#endif // VENUS_RENDERSYSTEM_HPP