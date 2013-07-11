#ifndef SIMPLEJOBFACTORY_H
#define SIMPLEJOBFACTORY_H

#include "jobfactory.hpp"
#include "common.hpp"
#include "simplejob.hpp"

namespace GM {
  class SimpleJobFactory : public JobFactory {
  public:
    SimpleJobFactory(): isReady(false) {}
    ~SimpleJobFactory();
    
    VIRTUAL Job* createInstance();
  private:
    bool isReady;
  };

  inline SimpleJobFactory::~SimpleJobFactory() {
    //    SimpleJob::freeme();
  }

  inline Job* SimpleJobFactory::createInstance() {
    if (!isReady) {
      SimpleJob::initialize();
      isReady = true;
    }
    
    return new SimpleJob();
  }
}

#endif
