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

#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "transform.hpp"
extern "C" {
  #include "spu/primitive.h"
}

namespace GM {
  class Primitive {
  public:
    virtual int transform(const Transform&) = 0;
    virtual int serialize(void*) = 0;
  };
}

#endif
