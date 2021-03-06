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
#include "global.hpp"

namespace GM {
  Thread::thread_mutex Global::mutexRay;
  Thread::thread_mutex Global::mutexJob;
  Thread::thread_mutex Global::mutexOut;  
  Thread::thread_mutex Global::mutexStatus;  
  Thread::thread_mutex Global::mutexPending;  

  Thread::thread_cond  Global::cond_jobber;
  Thread::thread_cond  Global::cond_accel;
  Thread::thread_cond  Global::cond_integ;
  Thread::thread_cond  Global::cond_stop;
  Thread::thread_mutex Global::mutex_jobber;
  Thread::thread_mutex Global::mutex_accel;
  Thread::thread_mutex Global::mutex_integ;
  Thread::thread_mutex Global::mutex_stop;
  
  deque<Ray>      Global::rayQueue;
  queue<Job*>     Global::jobQueue;
  vector<Object*> Global::objects;
  queue<void*>    Global::outQueue;
  vector<Light*>  Global::lights;

  uint32_t Global::MAX_LEVEL;
  uint32_t Global::MAX_RAYS;
  uint32_t Global::pending_num = 0;

  int Global::global_initialize() {
    Thread::createMutex(&mutexRay);
    Thread::createMutex(&mutexJob);
    Thread::createMutex(&mutexOut);
    Thread::createMutex(&mutexStatus);
    Thread::createMutex(&mutexPending);

    Thread::createMutex(&mutex_stop);
    Thread::createMutex(&mutex_jobber);
    Thread::createMutex(&mutex_accel);
    Thread::createMutex(&mutex_integ);

    Thread::createCondVariable(&cond_jobber);
    Thread::createCondVariable(&cond_accel);
    Thread::createCondVariable(&cond_integ);
    Thread::createCondVariable(&cond_stop);

    return 0;
  }
  

  MULTITHREADS void Global::thread_pending() {
    Thread::thread_mutex_lock(&mutexPending);
    pending_num ++;
    PRINTF("PENDING: %d\n", pending_num);
    if (pending_num == 3) {
      PUTS("SIGNAL STOP");
      Thread::thread_mutex_lock(&Global::mutex_stop);
      Thread::thread_mutex_unlock(&Global::mutex_stop);
      Thread::thread_cond_signal(&cond_stop);
    }      
    Thread::thread_mutex_unlock(&mutexPending);

  }

  MULTITHREADS void Global::thread_release() {
    Thread::thread_mutex_lock(&mutexPending);
    pending_num --;
    Thread::thread_mutex_unlock(&mutexPending);
    PRINTF("PENDING: %d\n", pending_num);
  }

  /**
   * Add a ray into the queue globally. 
   * @r the new ray.
   * @return 0 - success, 1 - failed
   */  
  MULTITHREADS int Global::global_add_ray(Ray& r) {    
    if (Thread::thread_mutex_lock(&Global::mutexRay)) 
      ERROR("global_add_ray: lock mutex");
    rayQueue.push_back(r);
    
    if (Thread::thread_mutex_unlock(&Global::mutexRay))
      ERROR("global_add_ray: unlock mutex");
    return 0; 
  }
  
  /**
   * Pop a ray from the queue globally.
   * @param j The new job.
   * @param n the number to pop
   * @return 0 - sucess, 1 - failed.
   */   
  MULTITHREADS int Global::global_pop_ray(Ray* j, int n) {
    int s;
    Thread::thread_mutex_lock(&mutexRay);
    for (s = 0; s < n; s ++) {
      if (rayQueue.size() == 0)
	break;
      j[s] = rayQueue.front();
      rayQueue.pop_front();
    }
    Thread::thread_mutex_unlock(&mutexRay);
    return s;
  }

  /**
   * Add a job to the queue globally.
   * @param j The new job.
   * @return 0 - sucess, 1 - failed.
   */   
  MULTITHREADS int Global::global_add_job(Job* j) {
    Thread::thread_mutex_lock(&mutexJob);
    jobQueue.push(j);
    if (jobQueue.size() == 1)
      Thread::thread_cond_signal(&cond_jobber);
    Thread::thread_mutex_unlock(&mutexJob);
    return 0;
  }
  
  /**
   * Pop a job from the queue globally.
   * @param j The new job.
   * @return 0 - sucess, 1 - failed.
   */   
  MULTITHREADS int Global::global_pop_job() {
    Thread::thread_mutex_lock(&mutexJob);
    jobQueue.pop();
    Thread::thread_mutex_unlock(&mutexJob);
    return 0;
  }
    
  /**
   * Clean up everything global.
   */
  MULTITHREADS int Global::global_clean_up() {
    Thread::thread_mutex_destroy(&mutexRay);
    Thread::thread_mutex_destroy(&mutexJob);
    Thread::thread_mutex_destroy(&mutexStatus);
    Thread::thread_mutex_destroy(&mutexOut);

    Thread::thread_mutex_destroy(&mutex_stop);
    Thread::thread_mutex_destroy(&mutex_jobber);
    Thread::thread_mutex_destroy(&mutex_integ);
    Thread::thread_mutex_destroy(&mutex_accel);
    
    Thread::thread_cond_destroy(&cond_jobber);
    Thread::thread_cond_destroy(&cond_accel);
    Thread::thread_cond_destroy(&cond_integ);
    Thread::thread_cond_destroy(&cond_stop);
    
    assert(rayQueue.size() == 0);
    assert(jobQueue.size() == 0);
    assert(outQueue.size() == 0);
    
    Object* oi;
    while (objects.size()) {
      oi = objects.back();
      objects.pop_back();
      delete oi;
    }
           
    Light* l;
    while(lights.size()) {
      l = lights.back();
      lights.pop_back();
      delete l;
    }
    
    return 0;    
  }
  
  /**
   * Add an object globally.
   */   
  MULTITHREADS int Global::global_add_object(Object* o) {
    objects.push_back(o);
    return RETURN_OK;
  }
    
  /**
   * Add the pointer into the queue.
   */ 
  MULTITHREADS int Global::global_add_intersection(void* ptr) {
    Thread::thread_mutex_lock(&mutexOut);
    outQueue.push(ptr);
    if (outQueue.size() == 1)
      Thread::thread_cond_signal(&cond_integ);
    
    Thread::thread_mutex_unlock(&Global::mutexOut);
    return RETURN_OK;    
  }


  MULTITHREADS int Global::global_pop_intersection() {
    Thread::thread_mutex_lock(&mutexOut);
    outQueue.pop();
    Thread::thread_mutex_unlock(&mutexOut);    
    return RETURN_OK;
  }
  
  int Global::global_add_light(Light* l) {
    lights.push_back(l);
    return RETURN_OK;
  } 
}
