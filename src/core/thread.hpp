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
#ifndef THREAD_HPP
#define THREAD_HPP

#include "common.hpp"


extern "C" {
#include "pthread.h"
#include "string.h"
}

namespace GM {

  class Thread {
  public:
    typedef pthread_mutex_t thread_mutex;
    typedef pthread_t       thread_t;
    typedef pthread_cond_t  thread_cond;
   
    Thread();
    ~Thread();
 
    /**
     * The actual function ran in thread.
     */
    virtual int run() = 0;
      
    /**
     * The stop function to join the thread.
     */
    virtual int stop() = 0;
      
    /*
     * Start the thread.
     */
    int start();
      
  protected:
    thread_t thread_id;
    bool isStop;
    	
  public:
    /*=================== static approaches ===================== */      
    /**
     * Entry of Thread. It's a wrapper of the pthread entrance and will
     * call a member function of the instance.
     */
    static void* threadEntry(void *);
      
    /**
     * A warpper method of pthread_mutex_lock.
     */      
    static int thread_mutex_lock(thread_mutex *);
      
    /**
     * A warpper method of pthread_mutex_unlock.
     */      
    static int thread_mutex_unlock(thread_mutex *);
      
    /**
     * A warpper method of pthread_mutex_destroy.
     */      
    static int thread_mutex_destroy(thread_mutex *);
     
    /**
     * A warpper method of pthread_mutex_init.
     */            
    static int createMutex(thread_mutex *);
      
    /**
     * A wrapper method of pthread_thread_exit;
     */            
    static void thread_exit();
      
    /**
     * A wrapper method of thread join;
     */            
    static int thread_join(thread_t);


    /*================ Conditional Variable =============== */
    /**
     * A warpper method of pthread_cond_init.
     */      
    static int createCondVariable(thread_cond *);

      
    /**
     * A wrapper method of pthread_thread_exit;
     */            
    static int thread_cond_signal(thread_cond *);

    /**
     * A wrapper method of pthread_cond_wait. The method will also do the
     * locking of the mutex at first and release it later. 
     */
    static int thread_cond_wait(thread_cond *, thread_mutex *);

    /**
     * A warpper method of pthread_cond_destroy.
     */      
    static int thread_cond_destroy(thread_cond *);

  };
}

#endif /*THREAD_HPP_*/
