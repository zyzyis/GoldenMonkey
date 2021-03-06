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

#include "sphere.hpp"

namespace GM {
  
  Sphere::Sphere(Point _p, float r) 
  : Object(TYPE_SPHERE), p(_p), radius(r) {
    initBbox();
  }  

  void Sphere::initBbox() {
    // right handness
    bbox.lb.x = p.x - radius;
    bbox.lb.y = p.y - radius;
    bbox.lb.z = p.z + radius;
    
    bbox.rt.x = p.x + radius;
    bbox.rt.y = p.y + radius;
    bbox.rt.z = p.z - radius;
  }
  
  int Sphere::serialize(void* ptr) {
    if (ptr == NULL)
      return RETURN_FAIL;    
    spu_shape_t* sp = (spu_shape_t*)ptr;
    
    sp -> type = TYPE_SPHERE;
    sp -> shape.sphere.o.x = p.x;
    sp -> shape.sphere.o.y = p.y;
    sp -> shape.sphere.o.z = p.z;
    sp -> shape.sphere.radius = radius;
    return RETURN_OK;
  }
  
  Vector Sphere::getNormal(Point pos) {
    return pos - p;
  }
}
