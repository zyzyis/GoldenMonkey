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
#ifndef TRANSLATION_HPP_
#define TRANSLATION_HPP_

#include "transform.hpp"

namespace GM {
  class Translation : public Transform {
  public:
    Translation(float x, float y, float z):
      Transform(1.f, 0.f, 0.f, x,
          0.f, 1.f, 0.f, y,
          0.f, 0.f, 1.f, z,
          0.f, 0.f, 0.f, 1.f),
      off_x(x), off_y(y), off_z(z){}

    float X() const;
    float Y() const;
    float Z() const;

  private:
    float off_x, off_y, off_z;    
  };

  inline float Translation::X() const { return off_x; }
  inline float Translation::Y() const { return off_y; }
  inline float Translation::Z() const { return off_z; }

}

#endif /*TRANSLATION_HPP_*/
