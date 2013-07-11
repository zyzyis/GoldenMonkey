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
 
#include "simplejob.hpp"
#include "util/memorytool.hpp"

namespace GM {
  spu_shape_t* SimpleJob::shapes = NULL;
  spu_light_t* SimpleJob::lights = NULL;
  
  SimpleJob::SimpleJob() : Job(JOB_GENERIC) {
    uint32_t nray = _min(Global::rayQueue.size(), Global::MAX_RAYS);
    int tmps;

    tmps = _round16(sizeof(spu_ray_t) * nray);
    rays = (spu_ray_t*) MemoryTool::alloc(tmps);

    tmps = _round16((nray + 1) * sizeof(spu_intersection_t));
    out  = (spu_intersection_t*)MemoryTool::alloc(tmps);

    Ray *tmp = new Ray[nray];
    Global::global_pop_ray(tmp, nray);
    for (uint32_t i = 0; i < nray; i ++) {
      tmp[i].serialize(rays + i);
    }
    delete []tmp;

    // initialize the scene description
    scene[0] = (uint64_t)this;
    scene[1] = Global::objects.size();
    scene[2] = nray;
    scene[3] = Global::lights.size();
  }
    
  SimpleJob::~SimpleJob() {
    MemoryTool::free(rays, "simplejob destructor::rays");
    MemoryTool::free(out,  "simplejob destructor::out");
  }
    
  void SimpleJob::initialize() {
    int nobj = Global::objects.size();
    int nlgt = Global::lights.size();
    
    shapes = (spu_shape_t*)MemoryTool::alloc(nobj * sizeof(spu_shape_t));            
    lights = (spu_light_t*)MemoryTool::alloc(nlgt * sizeof(spu_light_t));
                        
    for (int i = 0; i < nobj; i ++) {
      Global::objects[i] -> serialize(shapes + i);
    }
        
    for (int i = 0; i < nlgt; i++) {
      Global::lights[i] -> serialize(lights + i);
    }
  }
    
  void SimpleJob::freeme() {
    MemoryTool::free(shapes, "simplejob free::shapes");
    MemoryTool::free(lights, "simplejob free::lights");
  }

  void SimpleJob::serialize(gordon_job_t* p) {
    p -> index  = jobType;
    p -> nin    = 4;
    p -> ninout = 0;
    p -> nout   = 1;
    p -> next   = 0;
    p -> nalloc = 0;

    p -> buffers[0] = (uint64_t)scene;
    p -> buffers[1] = (uint64_t)rays;            
    p -> buffers[2] = (uint64_t)shapes;
    p -> buffers[3] = (uint64_t)lights;
    p -> buffers[4] = (uint64_t)out;

    int nobj = scene[1];
    int nray = scene[2];
    int nlgt = scene[3];

    p -> ss[0].size = _round16(4 * sizeof(uint64_t));
    p -> ss[1].size = _round16(nray * sizeof(spu_ray_t));
    p -> ss[2].size = _round16(nobj * sizeof(spu_shape_t));
    p -> ss[3].size = _round16(nlgt * sizeof(spu_light_t));
    p -> ss[4].size = _round16((nray + 1) * sizeof(spu_intersection_t));
  }
}
