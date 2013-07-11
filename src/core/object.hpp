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

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "transform.hpp"
#include "bbox.hpp"
#include "point.hpp"
#include "material.hpp"

namespace GM {
  class Object {
    public:
      Object(int t) : type(t) {}
      Object(int t, Point l, Point r) : type(t), bbox(l, r) {}
      int getType();      
      Bbox getBbox();
      
      void setMaterial(const Material& m);
      const Material& getMaterial();
            
      virtual int serialize(void*) = 0;
      virtual void initBbox() = 0;
      virtual Vector getNormal(Point) = 0;
      
    protected:
      Transform world2object;
      int type;
      Bbox bbox;
      Material material;
  };
   
  inline int Object::getType() { return type; }
  inline Bbox Object::getBbox() { return bbox; }
  inline void Object::setMaterial(const Material& m) {
    material = m;
  }
  
  inline const Material& Object::getMaterial() {
    return material;
  }
}
#endif /*OBJECT_HPP*/
