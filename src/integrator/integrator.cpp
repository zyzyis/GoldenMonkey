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

#include "integrator.hpp"

namespace GM {   
  Integrator::Integrator(uint32_t resX_, uint32_t resY_)
    : resX(resX_), resY(resY_) {
    image = new Color[resX * resY];
  }

  Integrator::~Integrator() {
    delete file;
    delete []image;
  }
    
  int Integrator::run() {
    void* ptr;
    while(1) {      
      if (Global::outQueue.empty()) {
	PUTS("INTEG IS WAITING...");
	Global::thread_pending();
	Thread::thread_cond_wait(&Global::cond_integ, &Global::mutex_integ);
	Global::thread_release();
	PUTS("INTEG IS ACTIVE!");
      }

      if (isStop) {
	// Do a final clean up to syncrhonize those nasty threads
	// A better solution is needed to avoid this.
	while(Global::outQueue.size()) {
	  ptr = Global::outQueue.front();
	  shading(ptr);
	  Global::outQueue.pop();
	}
	break;
      }

      ptr = Global::outQueue.front();
      shading(ptr);
      Global::global_pop_intersection();
    }
    return RETURN_OK;  
  }

  void Integrator::writeFile(const char* filename) {
    Rgba* rgba_image = new Rgba[resX * resY];
        
    uint32_t i, j, o, d;
    for (i = 0; i < resY; i ++) {
      for (j = 0; j < resX; j++) {
	o = i * resX + j;
	d = (resY - i - 1) * resX + j;
	rgba_image[d] = image[o].toRgba();
      }
    }
       
    file = new RgbaOutputFile(filename, resX, resY, WRITE_RGBA);
    file -> setFrameBuffer(rgba_image, 1, resX);
    file -> writePixels(resY);

    delete[] rgba_image;
  }
    
  int Integrator::stop() {
    if (isStop == false) {
      isStop = true;
      Thread::thread_cond_signal(&Global::cond_integ);
      thread_join(thread_id);
    }
    PUTS("Integrator stopped");
    return RETURN_OK;
  }        
}
