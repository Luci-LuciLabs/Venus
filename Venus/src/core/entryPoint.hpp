#pragma once

#include "../venus.hpp"
#include "../logger/loggerMantle.hpp"

extern venusCore::appCore* venusCore::buildApp();

int main(int argc, char** argv){

  auto app = venusCore::buildApp();
  app->run();
  delete app;
}