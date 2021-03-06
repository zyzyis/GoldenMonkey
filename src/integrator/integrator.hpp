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

#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "common.hpp"
#include "core/thread.hpp"
#include "core/color.hpp"
#include "util/global.hpp"
extern "C" {
#include "spu/primitive.h"
}

#include <ImfRgbaFile.h>
using namespace Imf;

namespace GM {    
  class Integrator : public Thread {
  public:
    Integrator(uint32_t, uint32_t);
    ~Integrator();
    VIRTUAL int run();
    VIRTUAL int stop();
    virtual int shading(void*) = 0;
             
    void writeFile(const char*);
  protected:
    Color* image;
    uint32_t resX, resY;     
    RgbaOutputFile* file;
  };
}


#endif /*INTEGRATOR_*/
