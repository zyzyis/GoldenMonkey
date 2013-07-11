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
#ifndef GMRENDER_HPP_
#define GMRENDER_HPP_

#include "common.hpp"
#include "camera/camera.hpp"
#include "accelerator/accelerator.hpp"
#include "jobber/jobber.hpp"
#include "integrator/integrator.hpp"

namespace GM {
  class GMrender {
  public:
    ~GMrender();
    void setCamera(Camera*);
    void setAccelerator(Accelerator*);
    void setJobber(Jobber*);
    void setIntegrator(Integrator*);
    void writeToFile(const char*);
    int render();
            
  private:
    Camera      * camera;
    Accelerator *accel;
    Jobber      *jobber;
    Integrator  * si;        
  };

  inline void GMrender::writeToFile(const char* filename) {
    si -> writeFile(filename);
  }
                                                                            
  inline void GMrender::setCamera(Camera* p) { camera = p; }
  inline void GMrender::setAccelerator(Accelerator* p) { accel = p; }
  inline void GMrender::setJobber(Jobber* p) { jobber = p; }       
  inline void GMrender::setIntegrator(Integrator* p) { si = p; }
}

#endif /*GM_HPP_*/

