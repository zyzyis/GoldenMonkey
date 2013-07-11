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

#include "celljobber.hpp"
#include "util/global.hpp"
#include "core/thread.hpp"
#include <cstdlib>
using namespace std;

/*
 * Warpper class for gordon C callback functions
 */
static GM::CellJobber* thisJobber;
static void gordon_callback(void* p) {
  thisJobber -> callbackJob((uint64_t)p);
}

namespace GM {
  CellJobber::CellJobber(unsigned int _nspu): nSpu(_nspu), status(0) {
    joblist = gordon_alloc_jobs(nSpu, 0);
    gordon_init(nSpu);
    thisJobber = this;
  } 
 
  CellJobber::~CellJobber() {
    gordon_join();
    gordon_terminate();
    gordon_free(joblist);
  }

  int CellJobber::stop() {
    if (isStop == false) {
      isStop = true;
      Thread::thread_cond_signal(&Global::cond_jobber);
      thread_join(thread_id);
    }
    
    PUTS("CellJobber stopped.");
    return RETURN_OK;
  }
  
  int CellJobber::sendJob(void* id, Job* job) {    
    uint64_t n = (uint64_t)id;
    job -> serialize(joblist + n);

    gordon_pushjob(joblist + n, gordon_callback, (void*)n);
    
    Thread::thread_mutex_lock(&Global::mutexStatus);
    status ^= (1u << n);
    Thread::thread_mutex_unlock(&Global::mutexStatus);
    return RETURN_OK;
  }
  
  void CellJobber::callbackJob(uint64_t index) {
    //release SPU    
    PRINTF("job %ld is finished.\n", index);
    int out_index = joblist[index].nin + joblist[index].ninout;
    void* out = (void *)joblist[index].buffers[out_index];
    int ss_out = joblist[index].ss[out_index].size;
    uint64_t* scene = (uint64_t*)(joblist[index].buffers[0]);
    
    // a hack to release the memory
    void* tmp = (void *)MemoryTool::alloc(ss_out);
    MemoryTool::copy(out, tmp, ss_out);
    delete (Job *)scene[0];
    
    Global::global_add_intersection(tmp);

    Thread::thread_mutex_lock(&Global::mutexStatus);
    status ^= (1u << index);
    Thread::thread_mutex_unlock(&Global::mutexStatus);
  }

  bool CellJobber::isFree() {
    return (status == 0);
  }

  inline int CellJobber::getNext() {
    for (int i = 0; i < nSpu; i ++)
      if ((status & (1u << i)) == 0) {
	return i;
      }
    return -1;
  }
  
  int CellJobber::run() {
    Job* ptr;
    int id;
    while(1) {
      if (Global::jobQueue.empty()) {
	PUTS("JOBBER IS WAITING...");
	Global::thread_pending();
	Thread::thread_cond_wait(&Global::cond_jobber, &Global::mutex_jobber);
	Global::thread_release();
	PUTS("JOBBER IS ACTIVE!");
      }
      
      if (isStop) break;
      ptr = Global::jobQueue.front();

      /**
       * Keep looping is not a good solution.
       * A better solution is needed since the gordon_poll() is
       * not syncrhonized.
       */
      while((id = getNext()) == -1) {         
        PUTS("CellJobber is waitting for SPU");
	gordon_wait(1);
      }      

      PRINTF("sending a job to %d.\n", id); 
      if (sendJob((void*)id, ptr))
        ERROR("Cannot send job!");

      Global::global_pop_job();
    } 
    return RETURN_OK;   
  }    
}
