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
 
#ifndef GRIDJOB_HPP
#define GRIDJOB_HPP

#include "job.hpp"
#include "core/ray.hpp"
#include "core/intersection.hpp"

extern "C" {
#include "spu/primitive.h"
#include "spu/jobtype.h"
}

namespace GM {
  class GridJob : public Job {
    public:
      static const unsigned int MAX_RAYSNO = 1;
      
       /**
        * Constructor.
        */
       GridJob();
             
    	/**
    	 * Deconstructor.
    	 */
    	~GridJob();
    	
    	/**
    	 * Get the job data.
    	 * @param ptr pointer to the data space.
    	 * @return the size of the data.
    	 */
    	int getJob(void* ptr);
    	
    	/**
    	 * Get the number of rays.
    	 */
    	int getRayNo() const;
    	
    	/**
    	 * Static method for initialize the static members.
    	 * @param grid The grid data stucture.
    	 * @return 0 - OK, 1 - Failed. 
    	 */
    	static int initialize(Grid*);
    	
      /**
       * Serialize the data into the gordon job structure.
       * @param p The gordon structure wrote into.
       */   
      VIRTUAL void serialize(gordon_job_t* p);

      static void freeMemory();      
    private:
      ALIGN spu_ray_t *rays;	// Ray data
      ALIGN spu_intersection_t *out; // intersection
      int nRay;
            
      /**
       * static data, change until any grid scene is initialized.
       */
      static ALIGN float* scene;             // Scene descriptions.
      static ALIGN spu_shape_t* shapes; 		  // Object data
      static ALIGN unsigned long* overlap; // The overlap index for each voxel       
  };  
}
 
#endif /*GRIDJOB_HPP*/
