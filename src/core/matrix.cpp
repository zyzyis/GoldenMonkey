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
 
#include "matrix.hpp"
#include <cstring>
//#include <memory>
using namespace std;

namespace GM {
  Matrix4x4::Matrix4x4(float d01, float d02, float d03, float d04,
		       float d11, float d12, float d13, float d14,
		       float d21, float d22, float d23, float d24,
		       float d31, float d32, float d33, float d34)
  {
    mat[0] = d01; mat[1] = d02; mat[2] = d03; mat[3] = d04;
    mat[4] = d11; mat[5] = d12; mat[6] = d13; mat[7] = d14;
    mat[8] = d21; mat[9] = d22; mat[10] = d23; mat[11] = d24;
    mat[12] = d31; mat[13] = d32; mat[14] = d33; mat[15] = d34;
  }
  
  Matrix4x4::Matrix4x4(const float* _d, int len) {
    if ( len >= 16 ) {
      memcpy(mat, _d, 32 * 16);
    } else {
      memset(mat, 0, 32 * 16);
      memcpy(mat, _d, 32 * len);
    }
  }
    
  Matrix4x4::Matrix4x4() {
    memset(mat, 0, sizeof(16 * 32));
    for (int i = 0; i < 4; i ++) 
      mat[i * 4 + i] = 1.f;
  }
 
  float Matrix4x4::operator() (const unsigned int i) const {
    return mat[i];
  }

  Matrix4x4 Matrix4x4::operator+ (const Matrix4x4& m) const {
    float tmp[16];
  
    for (int i = 0; i < 16; i ++)
      tmp[i] = mat[i] + m(i);
  
    return Matrix4x4(tmp, 16);
  }

  Matrix4x4 Matrix4x4::operator- (const Matrix4x4& m) const {
    float tmp[16];
  
    for (int i = 0; i < 16; i ++)
      tmp[i] = mat[i] - m(i);
  
    return Matrix4x4(tmp, 16);
  }
  
  Matrix4x4 Matrix4x4::operator* (const Matrix4x4& m) const {
    return Matrix4x4();	       // TODO
  }

  Matrix4x4 Matrix4x4::inverse() {
    return *this;
  }

  void Matrix4x4::print() {
    for (int i = 0; i < 16; i += 4) {
      printf("%4.2f %4.2f %4.2f %4.2f\n", mat[i], mat[i + 1], mat[i + 2], mat[i + 3]);
    }
  }
}
