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

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdio>
using std::printf;

namespace GM {
  class Matrix4x4 {
  public:
    Matrix4x4(float d01, float d02, float d03, float d04,
	      float d11, float d12, float d13, float d14,
	      float d21, float d22, float d23, float d24,
	      float d31, float d32, float d33, float d34);
    Matrix4x4(const float*, int);
    Matrix4x4();
    
    Matrix4x4 inverse();
    Matrix4x4 operator+ (const Matrix4x4&) const;
    Matrix4x4 operator- (const Matrix4x4&) const;
    Matrix4x4 operator* (const Matrix4x4&) const;
    float operator() (const unsigned int) const;
    
    void print();
    
  private:
    float mat[16];
    float inv[16];
  };
}

#endif
