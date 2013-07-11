/*
 * Copyright 2007 Si Yin. <Golden Monkey>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#ifndef _SAMPLER_H
#define _SAMPLER_H

#include "common.hpp"

namespace GM {
  class Sampler {
  public:
    Sampler(uint32_t xs, uint32_t xe, uint32_t ys, uint32_t ye, uint32_t s): 
      xstart(xs), xend(xe), ystart(ys), yend(ye), spp(s) {}

    virtual bool nextSample(float&, float&, uint32_t&) = 0;
    uint32_t getPixelX() const;
    uint32_t getPixelY() const;

  protected:
    uint32_t xstart, xend, ystart, yend;
    uint32_t spp;
  };
  
  inline uint32_t Sampler::getPixelX() const { return xend - xstart + 1; }
  inline uint32_t Sampler::getPixelY() const { return yend - ystart + 1; }
}

#endif
