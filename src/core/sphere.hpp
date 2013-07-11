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
#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "ray.hpp"
#include "point.hpp"
#include "object.hpp"
#include "common.hpp"
#include "vector.hpp"

namespace GM {
  class Sphere : public Object {
    public:
      Sphere(Point, float);
      VIRTUAL int serialize(void*);
      VIRTUAL void initBbox();
      VIRTUAL Vector getNormal(Point p); 
    private:
      Point p;
      float radius;
  };
}

#endif /*SPHERE_HPP_*/
