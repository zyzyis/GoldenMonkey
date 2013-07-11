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
#include "gm.hpp"
#include "util/global.hpp"

#ifdef PROFILE
extern uint64_t time_start;
extern uint64_t init_end;
#endif


namespace GM {
  GMrender::~GMrender() {    
    delete jobber;     
    delete camera;
    delete accel;
    delete si;
    
    Global::global_clean_up();            
  }
      
  int GMrender::render() {
    Global::global_initialize();
    camera -> generateRay();
    accel -> warm_up();

#ifdef PROFILE
    __time__(init_end);
#endif
    
    Thread::thread_mutex_lock(&Global::mutex_stop);
    si -> start();
    jobber -> start();
    accel -> start();

    // VERY NASTY CRITICAL SECTION HERE!
    // READ CAREFULLY FOR THE SYNCHRONIZING ORDERS
    while(1) {
      PUTS("WAITING...");
      Thread::thread_cond_wait(&Global::cond_stop, &Global::mutex_stop);
      Thread::thread_mutex_unlock(&Global::mutex_stop);
      PUTS("OUT");

      if (Global::rayQueue.size()) {
	Thread::thread_cond_signal(&Global::cond_accel);
	continue;
      } else if (jobber -> isFree())
	break;

      Thread::thread_mutex_lock(&Global::mutex_stop);
      gordon_wait(1);
    }

    PUTS("OUT OF LOOP");
    accel -> stop();
    si -> stop();
    jobber -> stop();

    return RETURN_OK;
  }
}
