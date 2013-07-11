/**
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
 
#ifndef VECTORJOB_HPP
#define VECTORJOB_HPP

#include "job.hpp"
#include "common.hpp"
extern "C" {
#include "spu/primitive.h"
}
#include "util/global.hpp"

namespace GM {
  class VectorJob : public Job {
  public:
    VectorJob();
    ~VectorJob();

    static void initialize();
    static void free();
    
    VIRTUAL void serialize(gordon_job_t* p);
                                
  private:
    ALIGN float    *rays;
    ALIGN float    *out;
    ALIGN uint32_t *origin;
    ALIGN uint64_t scene[5];
    
    static ALIGN spu_shape_t* shapes;
    static ALIGN spu_light_t* lights;
  };
}

#endif /*VECTORJOB_H_*/
