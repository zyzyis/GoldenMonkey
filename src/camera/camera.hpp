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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "sampler/sampler.hpp"
#include "core/translation.hpp"
#include "core/vector.hpp"
#include "core/ray.hpp"

namespace GM {
  class Camera {
  public:
    Camera(const Translation, Vector, Vector, Sampler*,	
	         float, float);
    
    ~Camera();
    
    virtual int generateRay() = 0;

  protected:
    Transform camera2world, raster2screen, screen2camera;
    Sampler* sampler;
    float near, far;
  };
}

#endif
