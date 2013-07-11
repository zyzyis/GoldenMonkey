/*
 * Copyright 2007 Si Yin. <Golden Monkey>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distribute
 * d on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */
#ifndef SIMPLEINTEGRATOR_HPP_
#define SIMPLEINTEGRATOR_HPP_

#include "common.hpp"
#include "integrator.hpp"

namespace GM {
    class SimpleIntegrator : public Integrator {
        public:
            SimpleIntegrator(uint32_t, uint32_t);
            VIRTUAL int shading(spu_intersection_t);
    };
    
    
    inline SimpleIntegrator::SimpleIntegrator(uint32_t x, uint32_t y) :
        Integrator(x, y) {}
}

#endif /*SIMPLEINTEGRATOR_HPP_*/
