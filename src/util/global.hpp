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

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "common.hpp"
#include "core/thread.hpp"
#include "core/ray.hpp"
#include "core/object.hpp"
#include "core/light.hpp"
#include "jobber/job.hpp"
#include "util/memorytool.hpp"

extern "C" {
#include "spu/primitive.h"
}

#include <deque>
#include <queue>
#include <vector>
using namespace std;

namespace GM {
  
  class Global {
  public:
    /**
     * Global traits
     */
    typedef vector<Object*>::iterator ObjectIterator; 
      
    /**
     * Global variables for mutlithreads share.
     */
    static Thread::thread_mutex mutexRay;
    static Thread::thread_mutex mutexJob;
    static Thread::thread_mutex mutexOut;
    static Thread::thread_mutex mutexStatus;
    static Thread::thread_mutex mutexPending;

    static Thread::thread_cond  cond_jobber;
    static Thread::thread_cond  cond_accel;
    static Thread::thread_cond  cond_integ;
    static Thread::thread_cond  cond_stop;
    static Thread::thread_mutex mutex_jobber;
    static Thread::thread_mutex mutex_accel;
    static Thread::thread_mutex mutex_integ;
    static Thread::thread_mutex mutex_stop;
    
    static deque<Ray> rayQueue;
    static queue<Job*> jobQueue;
    static vector<Object*> objects;
    static queue<void*> outQueue; 
    static vector<Light*> lights;

    static uint32_t MAX_LEVEL;
    static uint32_t MAX_RAYS;

    static uint32_t pending_num;

    static void thread_pending();
    static void thread_release();
    
    static int global_initialize();
    /**
     * Add a ray into the queue globally. 
     * @r the new ray.
     * @return 0 - success, 1 - failed
     */  
    MULTITHREADS static int global_add_ray(Ray& r);
     
    /**
     * Add a job to the queue globally.
     * @param j The new job.
     * @return 0 - sucess, 1 - failed.
     */   
    MULTITHREADS static int global_add_job(Job* j);
      
    /**
     * Pop a job from the queue globally.
     * @param j The new job.
     * @return 0 - sucess, 1 - failed.
     */   
    MULTITHREADS static int global_pop_job();
      
    /**
     * Pop a ray from the queue globally.
     * @param j The new job.
     * @return 0 - sucess, 1 - failed.
     */   
    MULTITHREADS static int global_pop_ray(Ray* j, int n);
      
    /**
     * Clean up everything global.
     */
    MULTITHREADS static int global_clean_up();
      
    /**
     * Global add objects
     */
    MULTITHREADS static int global_add_object(Object*);
       
    /**
     * Add the pointer into the queue.
     */ 
    MULTITHREADS static int global_add_intersection(void* ptr);    
    
    /**
     * Pop up an intersection pointer.
     */
    MULTITHREADS static int global_pop_intersection();

    /**
     * Addin a light object
     */
    static int global_add_light(Light*);                           
  };  
  

}
#endif /*GLOBAL_HPP_*/
