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

#ifndef CELLJOBBER_HPP
#define CELLJOBBER_HPP

#include "jobber.hpp"
#include "common.hpp"

extern "C" {
#include <stdint.h>
#include "spu/primitive.h"
#include "gordon.h"
#include "pthread.h"
}

namespace GM {
  class CellJobber : public Jobber {
  public:
    /**
     * constructor.
     * @param nspu_ : the number of spu actually used.
     */
    CellJobber(unsigned int nspu_);
      
    /**
     * deconstructor.
     */
    ~CellJobber();
      
    /**
     * send the job to the spu.
     * @param id The SPU id in the joblist.
     * @param job The job data.
     * @return 0 - success, 1 - failed.
     */
    VIRTUAL int sendJob(void* id, Job* job);
      
    /**
     * The actual function ran in thread.
     */    
    VIRTUAL int run();
      
    VIRTUAL int stop();
      
    /**
     * Function to be called whenever job is done.
     */
    void callbackJob(uint64_t index);
      
    /**
     * Get the next available SPU id.
     */
    int getNext();

    bool isFree();

  private:          
    gordon_job_t* joblist;
    int nSpu;
    uint8_t status;
  };  
}
#endif /*CELLJOBBER_HPP*/
