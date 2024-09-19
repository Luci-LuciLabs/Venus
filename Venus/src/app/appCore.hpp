#pragma once

#include "../window/windowCore.hpp"
#include "../pipeline/pipelineCore.hpp"
#include "../../resources/config/configCore.hpp"

#include <memory>
#include <vector>
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
    windowCORE window_core{resolution_width, resolution_height, app_name};

    pipelineCORE pipeline_core{"bin/shaders/simpleVertShader.vert.spv", "bin/shaders/simpleFragShader.frag.spv"};
};