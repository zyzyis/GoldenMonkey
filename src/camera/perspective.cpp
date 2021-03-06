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

#include "perspective.hpp"
#include "core/point.hpp"
#include "util/global.hpp"

namespace GM { 
  
  Perspective::Perspective(const Translation& t, 
			   Vector lookat,
			   Vector up,	 
			   Sampler* _sampler,
			   float left,
			   float right,
			   float top,
			   float bottom,
			   float _near,	 
			   float _far):
    Camera(t, lookat, up, _sampler, _near, _far) {

    int px = sampler -> getPixelX();
    int py = sampler -> getPixelY();

    float width = right - left;
    float height = top - bottom;
       
    raster2screen.setMatrix(Matrix4x4(width/(float)px, 0.f, 0.f, left,
				      0.f, height/(float)py, 0.f, bottom,
				      0.f, 0.0f, 1.0f, 0.0f,
				      0.f, 0.f, 0.f, 1.f));

    screen2camera.setMatrix(Matrix4x4(1.f, 0.f, 0.f, 0.f,
				      0.f, 1.f, 0.f, 0.f,
				      0.f, 0.f, 1.f, near,
				      0.f, 0.f, 0.f, 1.f));
  }

  int Perspective::generateRay() {
    Point tmp(0.f, 0.f, 1.f);
    Point o(0.f, 0.f, 0.f);
    Vector d(0.f, 0.f, 1.f);
    Ray r;

#ifdef RAY_PRINT
    int count = 0;
#endif

    while(sampler -> nextSample(tmp.x, tmp.y, r.origin)) { 
      tmp.transform(raster2screen);
      r.o = o;
      r.d.x = tmp.x;
      r.d.y = tmp.y;
      r.d.z = 1.0f;
      r.transform(screen2camera);
      r.transform(camera2world);
      r.round = 0;
      r.type = DIFFUSE_RAY;
      r.coef = 0.0f;
      
#ifdef RAY_PRINT
      printf("Ray: %d Origin: %.3f %.3f %.3f Dir: %.3f %.3f %.3f\n", 
	     count++, r.o.x, r.o.y, r.o.z, r.d.x, r.d.y, r.d.z);
#endif
      Global::rayQueue.push_back(r);
      //      Global::rayQueue.push_back(r);
    } // end while
    return RETURN_OK;
  }  
}
