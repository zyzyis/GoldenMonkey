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

#ifndef _STRATIFIEDSAMPLER_HPP
#define _STRATIFIEDSAMPLER_HPP

#include "sampler.hpp"


namespace GM {
  class StratifiedSampler : public Sampler {
  public:
    StratifiedSampler(uint32_t xs,	// start range in X axis
		              uint32_t xe,  // end range in X axis
		              uint32_t ys,  // start range in Y axis
		              uint32_t ye,  // end range in Y axis
		              uint32_t s):  // samples per pixel 
      Sampler(xs, xe, ys, ye, s) {
      xpos = xs; ypos = ys; count = 0;
    }
    
    bool nextSample(float&, float&, uint32_t&);
    
  private:
    uint32_t xpos, ypos;
    uint32_t count;
  };
}

#endif
