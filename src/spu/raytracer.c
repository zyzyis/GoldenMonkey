#include <stdio.h>
#include "../gordon/gordon.h"

#define NSPU 5

void callback_func(void * arg) {
    printf("Hi, it's call back\n");
}

int main() {
  struct gordon_ppu_job_s *rayjob = gordon_alloc_jobs(10, 0);
  int i = 0;    
  gordon_init(NSPU);
    
  for (i = 0; i < 10; i ++) {
    rayjob[i].index  = FUNC_RAY0;
    rayjob[i].nin    = 0;
    rayjob[i].ninout = 0;
    rayjob[i].nout   = 0;
    rayjob[i].next   = 0;
    
    if( gordon_pushjob_new(rayjob + i, callback_func, NULL)) {
        fprintf(stderr, "Oops, something wrong with the hello world\n");		
    }
  }
    
  //wait until all jobs have finished
  gordon_join(0); 
  gordon_terminate();	
  free(rayjob);
    
  return 0;
}

