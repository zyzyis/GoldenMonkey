#ifndef REDUCEINTEGRATOR_HPP
#define REDUCEINTEGRATOR_HPP

#include "common.hpp"
#include "integrator.hpp"


namespace GM {
  class ReduceIntegrator : public Integrator {
  public:
    ReduceIntegrator(uint32_t x, uint32_t y);

    VIRTUAL int shading(void* ptr);
  };

  inline ReduceIntegrator::ReduceIntegrator(uint32_t x, uint32_t y):
    Integrator(x, y) {}
}

#endif
