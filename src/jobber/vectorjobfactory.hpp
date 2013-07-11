#ifndef VECTORJOBFACTORY_H
#define VECTORJOBFACTORY_H

#include "jobfactory.hpp"
#include "vectorjob.hpp"
#include "common.hpp"

namespace GM {
  class VectorJobFactory : public JobFactory {
  public:
    VectorJobFactory(): isReady(false) {}
    ~VectorJobFactory();
    VIRTUAL Job* createInstance();
    
  private:
    bool isReady;
  };

  inline VectorJobFactory::~VectorJobFactory() {
    VectorJob::free();
  }

  inline Job* VectorJobFactory::createInstance() {
    if (!isReady) {
      VectorJob::initialize();
      isReady = true;
    }
    return new VectorJob();
  }
}

#endif
