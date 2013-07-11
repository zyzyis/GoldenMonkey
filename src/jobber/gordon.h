#ifndef GORDON_H
#define GORDON_H

#include <stdint.h>
#include <stdlib.h> //free

#define GORDON_MAX_STREAMS 12 //keep this a multiple of 4 because of alignment
#define GORDON_MAX_STRIDES 2048 //dma list size limit
#define GORDON_MAX_STRIDE_SIZE 16384 //dma list element size limit
#define GORDON_MAX_STRIDE_GAP 16384 //we have 10 bits left
#define GORDON_MAX_SPUS 8
#define GORDON_MAX_PENDING 4 //max # jobs pending at one SPU

//target size for all buffers of a job. The total shouldn't be bigger than
//targetsize * 2
#define GORDON_TARGETSIZE 51200
//maximum size, if you pass this something is wrong 
#define GORDON_MAX_SIZE (200*1024)

#define GORDON_INTERRUPTS_ONLY 0
#define GORDON_POLL_OUT_MBOX 1
#define GORDON_POLL_NORMAL_DMA 3
#ifndef GORDON_ACK
#define GORDON_ACK GORDON_POLL_OUT_MBOX
#endif

//This is a 32 bit structure that can be used instead of a sizes field to
//use strided data
typedef union {
    struct {
        //because the value 0 makes no sense and the maximum does,
        //all values are stored with one less
        uint32_t strided: 1; //upper bit indicates if striding is used
        uint32_t strides: 11; // strides + 1 == # strides, max 2048
        //size and gap must be multiples of 16: we don't store the lower 4 bits
        uint32_t size: 10; // (size + 1) << 4 == stride size, max 16k
        uint32_t gap: 10; // (gap + 1) << 4 == gap size, max 32k
    } stride;
    uint32_t size; //is valid when stride.strided == 0
} gordon_strideSize_t;
    
/* This structure describes a task to be done. It is used by PPU and SPU */
typedef struct gordon_ppu_job_s {
   uint8_t index; // which function ?
   uint8_t nalloc; //number of buffers to allocate @ spu
   uint8_t nin; // number of in buffers actually used 
   uint8_t ninout;// number of inout buffers actually used 
   uint8_t nout;// number of out buffers actually used

   uint8_t pad[3];
   //next job, last bit indicates if the next job
   //should reuse this job's buffers
   uint64_t next;

   uint64_t buffers[GORDON_MAX_STREAMS];
   gordon_strideSize_t ss[GORDON_MAX_STREAMS];
} gordon_job_t;

typedef gordon_job_t gordon_ppu_job_t;


typedef struct {   
   // function called back on the PPU when the job is finished
   void (*func)(void *); 
   void *arg; // the argument given the the callback function
} gordon_callback_t;

typedef struct { //make sure each joblist entry is aligned at 16 bytes
    uint64_t job;
    char pad[16 - sizeof(uint64_t)];
} gordon_joblist_item_t;

typedef struct {
    uint64_t ppu_joblist;
    uint64_t ppu_mespool;
    //char pad[16 - 2*sizeof(uint64_t)];
} gordon_init_args_t;

//main functions
void gordon_init (int nspus);
void gordon_terminate (void);
void gordon_pushjob (gordon_job_t *job, void (*callback)(void *),
                     void *callarg);
int gordon_poll (void);
int gordon_wait (unsigned int n);
void gordon_join (void);

//utility functions to handle some the internal structures
gordon_job_t *gordon_alloc_jobs (unsigned int njobs, int reuse_buffers);
#define gordon_free_jobs(joblist) free(joblist)

/** use this to allocate data that is passed to the spu (via gordon) */
void *gordon_malloc (size_t size);
#define gordon_free(ptr) free(ptr)

/** Use this to create a correct striding field */
gordon_strideSize_t gordon_stride_init
(unsigned int strides, unsigned int size, unsigned int distance);

#endif /* GORDON_H*/
