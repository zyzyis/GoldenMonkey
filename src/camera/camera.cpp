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

#include "camera.hpp"

namespace GM {
  
  Camera::Camera(const Translation translation, 
		 Vector lookat, 
		 Vector up,		
		 Sampler* _sampler,	
		 float _near,		
		 float _far):
    sampler(_sampler), near(_near), far(_far) {
    /**
     * Determine the coordinate system of the camera
     * the equation is first to find the projection
     * of up vector on the lookat vector and do a 
     * subtraction to get the second frame vector.
     * Do a cross product using the second vector and
     * lookat vector to get the third vector for the frame.
     */
    // default is right handness
#ifdef LEFT_HAND
    Vector v = lookat.cross(up);
    Vector u = v.cross(lookat);
#else        
    Vector v = up.cross(lookat);
    Vector u = lookat.cross(v);
#endif        

    lookat.normalize();
    v.normalize();
    u.normalize();
    
    float x = translation.X();
    float y = translation.Y();
    float z = translation.Z();
    
    camera2world.setMatrix(Matrix4x4(v.x, u.x, lookat.x, x,
				     v.y, u.y, lookat.y, y,
				     v.z, u.z, lookat.z, z,
				     0.f, 0.f, 0.f, 1.f));
    PRINTF("camera matrix:\n");
    PRINTF("%.2f %.2f %.2f %.2f\n", v.x, u.x, lookat.x, x);
    PRINTF("%.2f %.2f %.2f %.2f\n", v.y, u.y, lookat.y, y);
    PRINTF("%.2f %.2f %.2f %.2f\n", v.z, u.z, lookat.z, z);
    PRINTF("0 0 0 1\n");

  }
  
  Camera::~Camera() {
    delete sampler;
  }
}

