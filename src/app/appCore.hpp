#pragma once

#include "../window/windowCore.hpp"

#include <memory>
#include <vector>

namespace CORE{
  class appCORE{
    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      appCORE();
      ~appCORE();

      appCORE(const appCORE &) = delete;
      appCORE &operator=(const appCORE &) = delete;

      void run();
    
    private:
      windowCORE window_core{WIDTH, HEIGHT, "luci so kewlsies dis taiem fwfw ayayayaya"};
  };
}