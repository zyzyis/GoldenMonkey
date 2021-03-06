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
 
#ifndef ACCELERATOR_HPP
#define ACCELERATOR_HPP

#include "common.hpp"
#include "core/thread.hpp"
#include "util/global.hpp"
#include "jobber/jobfactory.hpp"


namespace GM {
  class Accelerator : public Thread {
  public:
    /**
     * Constructor
     */
    Accelerator(JobFactory *job):jobFactory(job) {}
    
    /**
     * Destructor
     */
    ~Accelerator();

    /**
     * Get the thread running
     */
    VIRTUAL int run();

    /**
     * Warm up the pipeline
     */
    VIRTUAL void warm_up();
    
    /**
     * Stop the thread
     */
    VIRTUAL int stop();
    
  protected:
    JobFactory *jobFactory;
  };
}

#endif /*ACCELERATOR_HPP_*/
