#include "accelerator.hpp"

namespace GM {
  Accelerator::~Accelerator() {
    stop();
    delete jobFactory;
  };
          
  VIRTUAL int Accelerator::stop() {
    if (isStop == false) {
      isStop = true;
      Thread::thread_cond_signal(&Global::cond_accel);     
      thread_join(thread_id);
      PUTS("Accelerator stopped.");
    }
           
    return 0;
  }


  VIRTUAL void Accelerator::warm_up() {
    while(1) {
      if (Global::rayQueue.size() < Global::MAX_RAYS)
	break;
      Global::global_add_job(jobFactory -> createInstance());
    }
  }

  VIRTUAL int Accelerator::run() {
    while(1) {
      if (Global::rayQueue.size() < Global::MAX_RAYS) {
	PUTS("ACCEL IS WAITING...");
	Global::thread_pending();
	Thread::thread_cond_wait(&Global::cond_accel, &Global::mutex_accel);
	Global::thread_release();
	PUTS("ACCEL IS ACTIVE!");
      }

      if (isStop) Thread::thread_exit();  
      
      Global::global_add_job(jobFactory -> createInstance());
    }
    return 0;
  }
}
