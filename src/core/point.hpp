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
#ifndef POINT_HPP_
#define POINT_HPP_

#include "primitive.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "transform.hpp"

namespace GM {
  class Point : public Primitive {
  public:
  	/**
  	 * Constructor.
  	 */
    Point(float _x = 0.f, float _y = 0.f, float _z = 0.f): 
      x(_x), y(_y), z(_z) {}

    Point(spu_point_t);
    
    /**
     * Overloaded operators.
     */
    Point operator+ (const Point&) const;
    Vector operator- (const Point&) const;
    Point& operator+= (const Point&);
    Point& operator-= (const Point&);
    Point operator/ (const float);
    
    /**
     * Transform.
     */
    VIRTUAL int transform(const Transform&);
    VIRTUAL int serialize(void* ptr);
    
    float x, y, z;
  };

  inline Point Point::operator+ (const Point& p) const {
    return Point(x + p.x, y + p.y, z + p.z);
  }

  inline Vector Point::operator- (const Point& p) const {
    return Vector(x - p.x, y - p.y, z - p.z);
  }

  inline Point& Point::operator+= (const Point& p) {
    x += p.x; y += p.y; z += p.z;
    return *this;
  }

  inline Point& Point::operator-= (const Point& p) {
    x -= p.x; y -= p.y; z -= p.z;
    return *this;
  }

  inline Point Point::operator/ (const float f) {
    float d = 1.f / f;
    return Point(x * d, y * d, z * d);
  }
  
  inline Point::Point(spu_point_t p) {
    x = p.x; y = p.y; z = p.z;
  }
}  

#endif /*POINT_HPP_*/
