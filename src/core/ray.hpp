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
#ifndef RAY_HPP_
#define RAY_HPP_

#include "transform.hpp"
#include "primitive.hpp"
#include "vector.hpp"
#include "point.hpp"

namespace GM {
  class Ray : public Primitive {
  public:
    /**
     * Construcor.
     * @_d direction vector.
     * @_o origin point.
     */
    Ray(Vector _d, Point _o, uint8_t _type):
      d(_d), o(_o), type(_type) {}

    Ray() : type(DIFFUSE_RAY) {}
    
    Ray(spu_ray_t);
    /**
     * Transform function.
     */
    int transform(const Transform&);
    
    /**
     * Serialize into SPU buffer.
     * @ptr the pointer to assign to
     * @return 0 - sucess, 1 - failed.
     */
    int serialize(void* ptr);
    
    /**
     * The direction of the ray.
     */
    Vector d;
    
    /**
     * The origin of the ray.
     */
    Point o;
    
    /**
     * Round number.
     */
    uint8_t round;
    
    /**
     * The type of the ray.
     */
    uint8_t type;
    
    /**
     * Coefficient of the ray.
     */
    float coef;
    
    /**
     * Origin index of the ray.
     */
    uint32_t origin;
  };
  
  inline Ray::Ray(spu_ray_t r): d(r.d), o(r.o), round(r.round), 
				type(r.type), coef(r.coef), origin(r.origin) {}
}

#endif /*RAY_HPP_*/
