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

#ifndef _ORTHOCAMERA_H
#define _ORTHOCAMERA_H
#include "common.hpp"
#include "camera.hpp"
#include "sampler/sampler.hpp"

namespace GM {
  // TODO: make camera to be thread
  
  class OrthoCamera : public Camera {
  public:
  	/**
  	 * Constructor.
  	 * @param t The position of camera.
  	 * @param lookat The lookat vector.
  	 * @param up The up vector.
  	 * @param _sampler, The sampler.
  	 * @param left, The left boundary.
  	 * @param right, The right boundary.
  	 * @param top, The up boundary.
  	 * @param bottom, The bottom boundary.
  	 * @param near, The near plane.
  	 * @param far, The far plane.
  	 */
    OrthoCamera(const Translation& t, 
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
