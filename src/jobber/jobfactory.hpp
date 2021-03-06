#ifndef JOBFACTORY_H
#define JOBFACTORY_H

#include "job.hpp"

namespace GM {
  class JobFactory {
  public:
    virtual Job* createInstance() = 0;
  };
}
#endif
