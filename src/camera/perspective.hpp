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

#ifndef _PERSPECTIVE_H
#define _PERSPECTIVE_H
#include "common.hpp"
#include "camera.hpp"
#include "sampler/sampler.hpp"

namespace GM {
  class Perspective : public Camera {
  public:
    Perspective(const Translation& t, 
		Vector lookat, 
		Vector up, 
		Sampler* _sampler,
		float left, 
		float right, 
		float top, 
		float bottom, 
		float _near, 
		float _far);

    VIRTUAL int generateRay();
  };
}
#endif
