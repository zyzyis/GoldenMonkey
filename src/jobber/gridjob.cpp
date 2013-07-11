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
 
#include "gridjob.hpp"
#include "util/memorytool.hpp"
#include "util/global.hpp"

namespace GM {
  /* Static Members */
  float* GridJob::scene = NULL;
  spu_shape_t* GridJob::shapes = NULL;
  Grid::BitFlag* GridJob::overlap = NULL;
  
  GridJob::GridJob() : Job(JOB_GRID) {
    nRay = _min(Global::rayQueue.size(), MAX_RAYSNO);
    rays = (spu_ray_t*) MemoryTool::memoryAlloc(
                          sizeof(spu_ray_t) * nRay); 
    out = (spu_intersection_t*) MemoryTool::memoryAlloc(
                          sizeof(spu_intersection_t) * nRay);
    
    Ray tmp;
    for (int i = 0; i < nRay; i ++) {
      Global::global_pop_ray(&tmp);
      tmp.serialize(rays + i);
    } // end while      
  }	
	
  GridJob::~GridJob() {
    MemoryTool::memoryFree(rays);
    MemoryTool::memoryFree(out);
  }
	
  int GridJob::getJob(void* ptr) {
    int size = nRay * sizeof(spu_ray_t);
		
    ptr = rays;
    return size;
  }

  	
  int GridJob::initialize(Grid* grid) {
    freeMemory();

    // Modifying the basic scene buffer
    scene = (float*) MemoryTool::memoryAlloc(9 * sizeof(float));
    Bbox b = grid -> getBbox();
    scene[0] = b.lb.x;
    scene[1] = b.lb.y;
    scene[2] = b.lb.z;
    scene[3] = b.rt.x;
    scene[4] = b.rt.y;
    scene[5] = b.rt.z;
    scene[7] = (float)grid -> getVoxelNo();
    scene[8] = (float)grid -> getObjectNo();

    int nObject = grid -> getObjectNo();
    int nVoxel = grid -> getVoxelNo();
    overlap = (Grid::BitFlag*)MemoryTool::memoryAlloc(nVoxel * sizeof(Grid::BitFlag));
    shapes  = (spu_shape_t*) MemoryTool::memoryAlloc(nObject * sizeof(spu_shape_t));
		    
    Grid::ObjectArray objects = grid -> getObjects();
    for (int i = 0; i < nObject; i ++)
      objects[i] -> serialize( shapes + i );

    for (int j = 0; j < nVoxel; j ++)
      overlap[j] = (*grid)(j);  
    
#ifdef GRIDJOB_PRINT 
    puts("================== GridJob Static ================");
    printf("Scene:");
    for (int i = 0; i < 8; i++) {
      printf("%.2f ", scene[i]);
    }
    printf("\nBitFlag:\n");
    for (int j = 0; j < nVoxel; j ++) {
      printf("Voxel %d: %X\n", j, overlap[j]);
    }
    puts("=================================================");
#endif

    return 0;    
  }
  
  void GridJob::serialize(gordon_job_t *p) {
    scene[6] = nRay;
    p -> index = 7; //jobType;
    p -> nin = 5;
    p -> ninout = 0;
    p -> nout = 1;
    p -> next = 0;
    p -> nalloc = 0;

    p -> buffers[0] = (uint64_t)rays;    
    p -> buffers[1] = (uint64_t)scene;
    p -> buffers[2] = (uint64_t)shapes;
    p -> buffers[3] = (uint64_t)overlap;
    p -> buffers[4] = (uint64_t)&lights;
    p -> buffers[5] = (uint64_t)&out;
    
    
    p -> sizes[0] = nRay * sizeof(spu_ray_t);
    p -> sizes[1] = 36; //sScene;
    p -> sizes[2] = (int)scene[8] * sizeof(spu_shape_t);
    p -> sizes[3] = (int)scene[7] * sizeof(Grid::BitFlag);
    p -> sizes[4] = (int)scene[9] * sizeof(spu_light_t);
    p -> sizes[5] = nRay * sizeof(spu_intersection_t);
        
#ifdef JOB_PRINT
    int n = (int)scene[6];
    puts("=================== Job ===================");
    printf("Total ray number: %d\n", nRay);
    for(int i = 0; i < nRay; i++) {
      printf("O:%.2f %.2f %.2f D: %.2f %.2f %.2f\n", 
	     rays[i].o.x, rays[i].o.y, rays[i].o.z, 
	     rays[i].d.x, rays[i].d.y, rays[i].d.z);    	
    }
    puts("===========================================");
#endif    
  }
  
  void GridJob::freeMemory() {
    MemoryTool::memoryFree(scene);
    MemoryTool::memoryFree(shapes);
    MemoryTool::memoryFree(overlap);
  }
}

