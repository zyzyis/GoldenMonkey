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
#include "thread.hpp"

namespace GM {
  Thread::Thread() : isStop(true) {}
	
  Thread::~Thread() {}
	
  int Thread::start() {
    isStop = false;
    return pthread_create(&thread_id, NULL, threadEntry, this);
  }
   
  void* Thread::threadEntry(void *ptr) {
    Thread* p = (Thread*)ptr;
    return (void*)(p -> run());
  }
    
  int Thread::thread_mutex_lock(thread_mutex *mutex) {
    return pthread_mutex_lock(mutex);
  }

  int Thread::thread_mutex_destroy(thread_mutex *mutex) {
    return pthread_mutex_destroy(mutex);
  }
  
  int Thread::thread_mutex_unlock(thread_mutex *mutex) {
    return pthread_mutex_unlock(mutex);
  }

  void Thread::thread_exit() {
    pthread_exit((void*)NULL);
  }
  
  int Thread::createMutex(thread_mutex *mutex) {
    return pthread_mutex_init(mutex, NULL);
  } 
  
  int Thread::thread_join(thread_t t) {
    return pthread_join(t, NULL);
  }  

  /* ===== conditional variable ====== */
  int Thread::thread_cond_destroy(thread_cond *cond) {
    return pthread_cond_destroy(cond);
  }
  
  int Thread::thread_cond_wait(thread_cond *cond, thread_mutex *mutex) {
    return pthread_cond_wait(cond, mutex);
  }

  int Thread::thread_cond_signal(thread_cond *cond) {
    return pthread_cond_signal(cond);
  }
  
  int Thread::createCondVariable(thread_cond *cond) {
    return pthread_cond_init(cond, NULL);
  }
}
