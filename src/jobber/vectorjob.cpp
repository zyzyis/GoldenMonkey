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
 
#include "vectorjob.hpp"
#include "util/memorytool.hpp"

namespace GM {
  spu_shape_t* VectorJob::shapes = NULL;
  spu_light_t* VectorJob::lights = NULL;

  VectorJob::VectorJob() : Job(JOB_VECTOR) {
    uint32_t nray = _min(Global::rayQueue .size(), Global::MAX_RAYS);
    rays   = (float *)MemoryTool::alloc(sizeof(float) * 6 * nray);
    out    = (float *)MemoryTool::alloc(sizeof(float) * 5 * (nray + 1));
    origin = (uint32_t *)MemoryTool::alloc(sizeof(uint32_t) * nray);

    Ray *tmp = new Ray[nray];
    
    Global::global_pop_ray(tmp, nray);

    for (uint32_t i = 0; i < nray; i ++) {
      rays[i]        = tmp[i].o.x;
      rays[nray + i] = tmp[i].o.y;
      rays[2 * nray + i] = tmp[i].o.z;
      rays[3 * nray + i] = tmp[i].d.x;
      rays[4 * nray + i] = tmp[i].d.y;
      rays[5 * nray + i] = tmp[i].d.z;

      origin[i] = tmp[i].origin;
    }

    delete []tmp;
    // initialize the scene description
    scene[0] = (uint64_t)this;
    scene[1] = Global::objects.size();
    scene[2] = Global::lights.size();
    scene[3] = nray;
    scene[4] = Global::MAX_LEVEL - 1;

  }

  VectorJob::~VectorJob() {
    MemoryTool::free(rays, "rays");
    MemoryTool::free(out, "out");
    MemoryTool::free(origin, "origin");
  }

  void VectorJob::initialize() {
    int nobj = Global::objects.size();
    int nlgt = Global::lights.size();
    
    shapes = (spu_shape_t*)MemoryTool::alloc(nobj * sizeof(spu_shape_t));            
    lights = (spu_light_t*)MemoryTool::alloc(nlgt * sizeof(spu_light_t));

    for (int i = 0; i < nobj; i ++)
      Global::objects[i] -> serialize(shapes + i);
          
    for (int i = 0; i < nlgt; i++)
      Global::lights[i] -> serialize(lights + i);
  }
  
  void VectorJob::free() {
    MemoryTool::free(shapes, "vectorjob free::shapes");
    MemoryTool::free(lights, "vectorjob free::lights");
  }

  void VectorJob::serialize(gordon_job_t* p) {
    p -> index  = jobType;
    p -> nin    = 5;
    p -> ninout = 0;
    p -> nout   = 1;
    p -> next   = 0;
    p -> nalloc = 0;

    // in buffer
    p -> buffers[0] = (uint64_t)scene;
    p -> buffers[1] = (uint64_t)rays;
    p -> buffers[2] = (uint64_t)origin;
    p -> buffers[3] = (uint64_t)shapes;
    p -> buffers[4] = (uint64_t)lights;

    // out buffer
    p -> buffers[5] = (uint64_t)out;
    
    int nobj = (int)scene[1];
    int nlgt = (int)scene[2];
    int nray = (int)scene[3];

    p -> ss[0].size = _round16(sizeof(uint64_t) * 5);           // scenes
    p -> ss[1].size = _round16(sizeof(float) * nray * 6);       // rays
    p -> ss[2].size = _round16(sizeof(uint32_t) * nray);        // origins
    p -> ss[3].size = _round16(sizeof(spu_shape_t) * nobj);     // shapes
    p -> ss[4].size = _round16(sizeof(spu_light_t) * nlgt);     // lights
    p -> ss[5].size = _round16(sizeof(float) * (nray + 1) * 5); // out
  }
}
