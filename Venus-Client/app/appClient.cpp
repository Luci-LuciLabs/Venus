#include "../../Venus/src/venus.hpp"

//appClient inherits from engine appCore and uses entrypoint to do so

class appClient : public venusCore::appCore{
  public:
    appClient(){};
    ~appClient(){};

};

venusCore::appCore* venusCore::buildApp(){
  return new appClient();
}
