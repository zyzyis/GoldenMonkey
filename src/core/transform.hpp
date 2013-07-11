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

#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "matrix.hpp"
#include "common.hpp"

namespace GM {
  class Transform {
  public:
    Transform(Matrix4x4& m): mat(m) {}
    Transform(float d01, float d02, float d03, float d04,
	      float d11, float d12, float d13, float d14,
	      float d21, float d22, float d23, float d24,
	      float d31, float d32, float d33, float d34):
      mat(d01, d02, d03, d04, 
	  d11, d12, d13, d14,
	  d21, d22, d23, d24, 
	  d31, d32, d33, d34) {}
    Transform() {}

    int setMatrix(const Matrix4x4&);
    Matrix4x4 getMatrix() const;
    
  protected:
    Matrix4x4 mat;
  };

  inline int Transform::setMatrix(const Matrix4x4& m) {
      mat = m; return RETURN_OK; 
    }
    
  inline Matrix4x4 Transform::getMatrix() const {
    return mat;
  }
}
    
#endif
