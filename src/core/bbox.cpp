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
  
#include "bbox.hpp"
#include "common.hpp"

namespace GM {  
  Bbox::Bbox(Point l, Point r): lb(l), rt(r) {}
  
  Bbox::Bbox() {};
   
  Bbox Bbox::unit(const Bbox& b) {
    Point l(_min(lb.x, b.lb.x), _min(lb.y, b.lb.y), _min(lb.z, b.lb.z));
    Point r(_max(rt.x, b.rt.x), _max(rt.y, b.rt.y), _max(rt.z, b.rt.z)); 
    return Bbox(l, r);
  }
    
  bool Bbox::isInside(const Point p) const {
    if (p.x < lb.x || p.x > rt.x || p.y < lb.y
        || p.y > rt.y || p.z > lb.z || p.z < rt.z)
      return false;
    return true;
  }
}
