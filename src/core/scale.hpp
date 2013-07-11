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
#ifndef SCALE_HPP_
#define SCALE_HPP_

#include "transform.hpp"

namespace GM {
  class Scale : public Transform {
  public:
    Scale(float x, float y, float z):
      Transform(x, 0.f, 0.f, 0.f,
          0.f, y, 0.f, 0.f,
          0.f, 0.f, z, 0.f,
          0.f, 0.f, 0.f, 1.f),
      scale_x(x), scale_y(y), scale_z(z) {}
        
    float X();
    float Y();
    float Z();

  private:
    float scale_x, scale_y, scale_z;
  };

  inline float Scale::X() { return scale_x; }
  inline float Scale::Y() { return scale_y; }
  inline float Scale::Z() { return scale_z; }

}

#endif /*SCALE_HPP_*/
