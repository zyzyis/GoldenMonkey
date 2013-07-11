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
#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include "transform.hpp"
#include "matrix.hpp"
#include "primitive.hpp"
#include "common.hpp"
#include <cassert>
#include <cmath>
using namespace std;

namespace GM {
  class Vector : public Primitive {
  public:
    /**
     * Constructor.
     */
    Vector(float _x = 0, float _y = 0, float _z = 0):
      x(_x), y(_y), z(_z) {}
    
    Vector(spu_vector_t);
    
    /**
     * Overloaded operators.
     */
    Vector operator* (const float) const;
    Vector operator/ (const float) const;
    Vector operator+ (const Vector&) const;
    Vector operator- (const Vector&) const;
    Vector& operator+= (const Vector&);
    Vector& operator-= (const Vector&);
    
    /**
     * Normalize vector.
     * @return always be true except vector is not valid.
     */
    int normalize();
    
    /**
     * Dot product.
     * @return the value of the formula.
     */
    float dot(const Vector&) const;
    
    /**
     * Cross product.
     * @return the result vector.
     */
    Vector cross(const Vector&) const;
    
    /**
     * Transform.
     */
    VIRTUAL int transform(const Transform&);
    
    /**
     * Serialize into spu buffer.
     * @ptr 
     * @return 0 - success, 1 - failed.
     */
    VIRTUAL int serialize(void* ptr);
    
    float length2() const;    
    float x, y, z;
  };
  
  inline int Vector::normalize() {
    if (x == 0 && y == 0 && z == 0)
      return RETURN_FAIL;
  	
    float det = 1.f / sqrt(x * x + y * y + z * z);
    x *= det; y *= det; z *= det;
    return RETURN_OK;
  }

  inline float Vector::dot(const Vector& v) const {
    return x * v.x + y * v.y + z * v.z;
  }

  inline Vector Vector::cross(const Vector& v) const{
    return Vector(y * v.z - z * v.y,
		  z * v.x - x * v.z,
		  x * v.y - y * v.x);       
  }    

  inline Vector Vector::operator* (const float b) const {
    return Vector(x * b, y * b, z * b);
  }

  inline Vector Vector::operator/ (const float b) const {
    assert(fabs(b) > INFINITY_MIN);

    float tmp = 1.f / b;
    return Vector(x * tmp, y * tmp, z * tmp);      
  }

  inline Vector Vector::operator+(const Vector& v) const {
    return Vector(x + v.x, y + v.y, z + v.z);
  }

  inline Vector Vector::operator-(const Vector& v) const {
    return Vector(x - v.x, y - v.y, z - v.z);
  }

  inline Vector& Vector::operator+=(const Vector&v) {
    x += v.x; y += v.y; z += v.z;
    return *this;
  }

  inline Vector& Vector::operator-=(const Vector& v) {
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
  }
    
  inline float Vector::length2() const {
    return x * x + y * y + z * z;
  }
  
  inline Vector::Vector(spu_vector_t t) {
    x = t.x; y = t.y; z = t.z;
  }
}

#endif /*VECTOR_HPP_*/
