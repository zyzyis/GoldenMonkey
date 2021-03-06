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
#include "whittedintegrator.hpp"
#include "core/material.hpp"
#include "util/global.hpp"
#include <cmath>

#ifdef PROFILE
extern double spu_time;
#endif

namespace GM {
  int WhittedIntegrator::shading(void* ptr) {
    spu_intersection_t *o = (spu_intersection_t *)ptr;
    int i = -1;
    while (1) {
      i ++;
      if (o[i].objid == 0)
	continue;

      if (o[i].objid == 0xFFFFFFFF) {
#ifdef PROFILE
	spu_time += (double)(o[i].shade);
#endif
	break;
      }
      real_shading(o[i]);
    }

    MemoryTool::free(o);

    return 0;
  }

  int WhittedIntegrator::real_shading(spu_intersection_t o) {
    int i;                
    Ray r;
    float dot, dist;
    vector<Light*>::iterator it;
    Object* object = Global::objects[o.objid - 1];
                
    // light, incident, normal, reflection and transmition
    Point p(o.rr.o);
    Vector L, R;
    Vector I(o.rr.d);
    Vector N(object -> getNormal(p));
    
    // material
    Material m = object -> getMaterial();
    N.normalize();
    
    if (o.rr.type == DIFFUSE_RAY && m.getDiffuse() != 0.0f) {
      // ambient light
      // image[o.rr.origin] = m.getColor() * 0.03f;
      for(it = Global::lights.begin(), i = 0; 
	  it != Global::lights.end(); it ++, i ++) {

        if (!(o.shade & (1u << i))) {
	  // compute diffuse color
	  L = (*it) -> pos - p;
	  dist = 1.f / L.length2();
	  L.normalize();    
	  dot = N.dot(L);
	  if (dot > 0.0f) {
	    // intersect at the front
	    dot *= m.getDiffuse() * dist;
	    image[o.rr.origin] += m.getColor() * ((*it) -> intensity * dot);
	  }

#ifdef SPECULAR_ENABLED
	  // compute specular color
	  L = (*it) -> pos - p;
	  dot = L.dot(N);
	  if (dot > 0) {                    
	    R = N * 2.0f - L / dot;
	    I.normalize();
	    R.normalize();
	    dot = R.dot(I);
	    if (dot > 0) {                        
	      dot = m.getSpecular() * powf(dot, 30.0f);
	      image[o.rr.origin] += (*it) -> c * dot;
	    }
	  }
#endif
	}
      }
    } // end if

#ifdef REFLECTION_ENABLED            
    // compute the reflection
    if (o.rr.round < Global::MAX_LEVEL && m.getReflection() != 0.0f) {
      // create the reflection ray
      dot = I.dot(N);
      if (dot > 0) {
        r = o.rr;
	// r.d = N * 2.0f - I / dot;
        r.coef = m.getReflection();
        r.type = REFLECTION_RAY;
      }
            
      Global::global_add_ray(r);
      if (Global::rayQueue.size() == Global::MAX_RAYS)
        Thread::thread_cond_signal(&Global::cond_accel);
    } // end if
                                            
    if (o.rr.type == REFLECTION_RAY) {
      image[o.rr.origin] += m.getColor() * o.rr.coef * 0.1;
      // another reflection ray
      if (o.rr.round < Global::MAX_LEVEL && m.getReflection() != 0.0f) {
        dot = I.dot(N);
        if (dot > 0) {
	  r = o.rr;        
	  // r.d = N * 2.0f - I / dot;
	  r.coef *= m.getReflection();
        }
                
        Global::global_add_ray(r);
      }
    }
#endif                
    return 0;
  }
}
