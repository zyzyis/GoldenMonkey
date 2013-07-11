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
#include "box.hpp"

namespace GM {
  Box::Box(Point l, Point r) : Object(TYPE_BOX, l, r){
  }
  
  void Box::initBbox() {}
  
  int Box::serialize(void* ptr) {
    if (ptr == NULL)
      return RETURN_FAIL;
      
    spu_shape_t* p = (spu_shape_t*)ptr;
    p -> type = type;
    p -> shape.box.lx = lb.x;
    p -> shape.box.ly = lb.y;
    p -> shape.box.lz = lb.z;
    p -> shape.box.rx = rt.x;
    p -> shape.box.ry = rt.y;
    p -> shape.box.rz = rt.z;
    return RETURN_OK;
  }    
}
