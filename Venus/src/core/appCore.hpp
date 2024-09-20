#pragma once

#include "windowCore.hpp"

namespace venusCore{
  class appCore{
    public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      appCore();
      virtual ~appCore();

      appCore(const appCore &) = delete;
      appCore &operator=(const appCore &) = delete;

      virtual void run();
    
    private:
      windowCore window_core{WIDTH, HEIGHT, "Luci so fuhg N KEWLSIES FWFW AYAYAYAYAYAYAYAYA >:3"};
  };  
}