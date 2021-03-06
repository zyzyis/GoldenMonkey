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

#include "point.hpp"

namespace GM {

  int Point::transform(const Transform& t) {
    Matrix4x4 mat = t.getMatrix();
    float tmp_x, tmp_y, tmp_z, w;

    tmp_x = mat(0) * x + mat(1) * y + mat(2) * z + mat(3);
    tmp_y = mat(4) * x + mat(5) * y + mat(6) * z + mat(7);
    tmp_z = mat(8) * x + mat(9) * y + mat(10) * z + mat(11);
    w = mat(12) * x + mat(13) * y + mat(14) * z + mat(15);        
    assert(w != 0.f);
    x = tmp_x / w; y = tmp_y / w; z = tmp_z / w;

    return RETURN_OK;
  }  

  int Point::serialize(void* ptr) {
    if (ptr == NULL)
      return RETURN_FAIL;
  	
    spu_point_t* p = (spu_point_t*)ptr;
    p -> x = x;
    p -> y = y;
    p -> z = z;
    return RETURN_OK;
  }
}
