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

#include "ray.hpp"
#include <cassert>
using namespace std;

extern "C" {
#include "spu/primitive.h"
}

namespace GM {
  int Ray::transform(const Transform& t) { 
    o.transform(t);   
    d.transform(t);
    return RETURN_OK;
  }
  
  int Ray::serialize(void* ptr) {
  	assert(ptr != NULL);
  	
  	spu_ray_t* p = (spu_ray_t*)ptr;
  	
  	d.serialize(&(p -> d));
  	o.serialize(&(p -> o));
  	p -> round = round;
    p -> origin = origin;
    p -> type = type;
    p -> coef = coef;
        
  	return RETURN_OK;
  }
}

