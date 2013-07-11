#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <libspe2.h>

#include "gordon.h"
#include "pputil.h"
#include <util/util.h>

// #define GORDON_DEBUG 1

#define EMPTY_ACK (GORDON_MAX_SPUS * GORDON_MAX_PENDING + 42)

// This is the automaton code that will run on the SPU
extern spe_program_handle_t gordon_automaton;

//the mutex protects all globals listed here:
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//busy list is allocated once and remains static
//wait list is doubled if it is full, size is always a power of 2
//The busy list might have gaps as jobs may be completed out of order
//An empty entry in the busy list should therefore always be set to NULL
//this doesn't apply to the wait list as it is a FIFO.
static unsigned int busy_next, busy_used, busy_size;
static unsigned int wait_next, wait_used, wait_size;
static gordon_joblist_item_t *busy_list;
static gordon_job_t **wait_list;
static gordon_callback_t *busy_callbacks, *wait_callbacks;

typedef struct {
    spe_context_ptr_t spe_id;
    pthread_t thread;
    unsigned int pending; //# pending jobs @ this spe
    unsigned int next_ack; // next acknowledgement slot
    volatile unsigned int *acks;
} gordon_spe_t;

static unsigned int spe_nused = 0, spe_next;
static gordon_spe_t spes[GORDON_MAX_SPUS];
static volatile unsigned int __attribute__ ((aligned(128)))
   spe_acks[GORDON_MAX_SPUS * (128/sizeof(unsigned int))];
static void *spe_thread_run (void *arg_void);

static int gordon_users = 0;

static void gordon_finished (unsigned int s, unsigned int j);
static void gordon_wait_locked (unsigned int n);

#if (GORDON_ACK == GORDON_INTERRUPTS_ONLY)
#include "gordon-intr.inc.c"
#else
#include "gordon-poll.inc.c"
#endif

void gordon_init(int nspus) 
{
    int result;
    unsigned u, v;
    char *s;
    
    ASSERT(sizeof(gordon_job_t) % 16 == 0);
    ASSERT(sizeof(spe_acks) == 128 * GORDON_MAX_SPUS);
        
    mutex_lock(&mutex);
    result = gordon_users;
    gordon_users++;
    mutex_unlock(&mutex);
    
    if (result > 0) return; //we had already been initialized
    
    spe_nused = min(spe_cpu_info_get(SPE_COUNT_PHYSICAL_SPES, -1), nspus);

    if (spe_nused > GORDON_MAX_SPUS) {
        printf("Increase GORDON_MAX_SPUS if you want to use %d spus!\n",
               spe_nused);
        spe_nused = GORDON_MAX_SPUS;
    }
    
    busy_next = busy_used = 0; //initialize busy and waitlist
    busy_size = spe_nused * GORDON_MAX_PENDING;
    busy_list = samalloc(busy_size * sizeof(*busy_list), 16);
    busy_callbacks = smalloc(busy_size * sizeof(*busy_callbacks));
    for (u = 0; u < busy_size; u++) busy_list[u].job = (uintptr_t)NULL;
    
    wait_next = wait_used = 0;
    wait_size = 16; //must be a power of 2
    wait_list = smalloc(wait_size * sizeof(*wait_list));
    wait_callbacks = smalloc(wait_size * sizeof(*wait_callbacks));

    switch(GORDON_ACK) {
    case GORDON_INTERRUPTS_ONLY: s = "interrupts"; break;
    case GORDON_POLL_OUT_MBOX: s = "out mbox"; break;
    case GORDON_POLL_NORMAL_DMA: s = "normal dma"; break;
    default: ASSERT(0); s = "INVALID MODE!"; break;
    }
    
    printf("Gordon: Using %d SPUs with %s\n", spe_nused, s);

    // Then we add one thread per SPU : they will run "automaton"
    for (u = 0; u < spe_nused; u++) {
        spes[u].spe_id = spe_context_create(SPE_EVENTS_ENABLE, NULL);
        if (spes[u].spe_id == NULL) {
            int errsv = errno;
            fprintf(stderr, "Gordon: Failed to create spe context: %s\n",
                    strerror(errno));
            exit(errsv);
        }

        spes[u].pending = spes[u].next_ack = 0;
        spes[u].acks = &spe_acks[u * (128/sizeof(unsigned int))];
        for (v = 0; v < GORDON_MAX_PENDING; v++) {
            spes[u].acks[v] = EMPTY_ACK;
        }

        result = pthread_create(&spes[u].thread, NULL, spe_thread_run,
                                (void *) (uintptr_t) u);
        if (result != 0) {
            int errsv = errno;
            fprintf(stderr, "Gordon: Failed to create spe thread: %s\n",
                    strerror(errno));
            exit(errsv);
        }
        
        //Wait for a message from the spu that it is ready. If we do not do
        //this the event thread is likely to crash the whole ps3 when
        //multiple SPUs are used!
        do {
            result = spe_out_mbox_read(spes[u].spe_id, &v, 1);
            if (result == -1 && errno == EAGAIN) continue;
            if (__builtin_expect(result < 0, 0)) {
                int errsv = errno;
                fprintf(stderr, "Gordon: Couldn't read mbox @ init, "
                        "status: %d, data = %x, error: %s\n", result, v,
                        strerror(errno));
                exit(errsv);
            }
        } while (result != 1);
        
        ASSERT(v == 0x42);
    }
    
#if (GORDON_ACK == GORDON_INTERRUPTS_ONLY)
    result = pthread_create(&event_thread, NULL, event_thread_run, NULL);
    if (result != 0) {
        int errsv = errno;
        perror("Gordon: Unable to create event thread");
        exit(errsv);
    }
#endif
}

void *spe_thread_run (void *arg_void) {
    gordon_init_args_t arguments __attribute__ ((aligned(16)));
    unsigned int spe_index = (uintptr_t) arg_void;
    spe_context_ptr_t spe_id = spes[spe_index].spe_id;
    unsigned int entry = SPE_DEFAULT_ENTRY;
    spe_stop_info_t stopinfo;
    int status;
    
    status = spe_program_load(spe_id, &gordon_automaton); ASSERT(status == 0);
    if (status != 0) {
        int errsv = errno;
        fprintf(stderr, "Gordon: Failed to load spe program: %s\n",
                strerror(errno));
        exit(errsv);
    }
    
    arguments.ppu_joblist = (uintptr_t) busy_list;
#if (GORDON_ACK == GORDON_POLL_NORMAL_DMA)
    arguments.ppu_mespool = (uintptr_t) spes[spe_index].acks;
#endif
    status = spe_context_run(spe_id, &entry, 0, &arguments,
                             NULL, &stopinfo);
    
    if (__builtin_expect(status != 0 || stopinfo.stop_reason != SPE_EXIT ||
                         stopinfo.result.spe_exit_code != 42, 0)) {
        printf("Gordon: SPE %p stopped abnormally; status = 0x%x\n",
               spe_id, status);
        printf("Gordon: SPE %p stopreason = 0x%x, code = %d, status = 0x%x\n",
               spe_id, stopinfo.stop_reason, stopinfo.result.spe_exit_code,
               stopinfo.spu_status);
    }
    return NULL;
}

static void __attribute__ ((unused)) print_chain (gordon_job_t *chain) {
    int id = 0;
    unsigned int u;
    do {
        unsigned int buf = 0;
        printf("%d: func index = %d, next = %p\n", id, chain->index,
               (void *) chain->next);
        
        printf("   nalloc = %d:", chain->nalloc);
        for (u = 0; u < chain->nalloc; u++, buf++) {
            printf(" %p/%x", (void *) chain->buffers[buf],
                   chain->ss[buf].size);
        }
        printf("\n   nin = %d:", chain->nin);
        for (u = 0; u < chain->nin; u++, buf++) {
            printf(" %p/%x", (void *) chain->buffers[buf],
                   chain->ss[buf].size);
        }
        printf("\n   ninout = %d:", chain->ninout);
        for (u = 0; u < chain->ninout; u++, buf++) {
            printf(" %p/%x", (void *) chain->buffers[buf],
                   chain->ss[buf].size);
        }
        printf("\n   nout = %d:", chain->nout);
        for (u = 0; u < chain->nout; u++, buf++) {
            printf(" %p/%x", (void *) chain->buffers[buf],
                   chain->ss[buf].size);
        }
        printf("\n");
        
        chain = (gordon_job_t *)(uintptr_t)(chain->next - (chain->next & 1));
        id++;
    } while (chain != NULL && id < 142);
}

/** Submits the job at busy_list[job_index] to the specified spe */
static void gordon_submit (unsigned int spe_index, unsigned int job_index) {
    int status;
#ifdef GORDON_DEBUG
    printf("Gordon: Submitting %d(%lx) to SPU %p\n", job_index,
           busy_list[job_index].job, spes[spe_index].spe_id);
#endif
    // print_chain(job);
    status = spe_in_mbox_write(spes[spe_index].spe_id, &job_index, 1,
                               SPE_MBOX_ANY_NONBLOCKING); //submit job to spe
    if (__builtin_expect(status != 1, 0)) {
        printf("Gordon: Couldn't write to input mailbox of SPE %p!\n",
               spes[spe_index].spe_id);
        printf("Gordon: status = %d, if -1 error msg: %s", status,
               strerror(errno));
        exit(errno);
    }
}

/** Add a job to the waiting queue */
static void gordon_wait_add (gordon_job_t *job, void (*callback)(void *),
                             void *callarg) {
    if (wait_used == wait_size) {
        wait_size *= 2;
#ifdef DEBUG
        printf("Gordon doubling wait list to 0x%x\n", wait_size);
#endif
        wait_list = srealloc(wait_list, wait_size * sizeof(*wait_list));
        wait_callbacks = srealloc(wait_callbacks,
                                  wait_size * sizeof(*wait_callbacks));
        //copy the first wait_next entries to the 2nd half
        //and update wait_next to keep the same ordening
        memcpy(&wait_list[wait_used], wait_list,
               wait_next * sizeof(*wait_list));
        memcpy(&wait_callbacks[wait_used], wait_callbacks,
               wait_next * sizeof(*wait_callbacks));
        wait_next += wait_used;
    }

#ifdef GORDON_DEBUG
    printf("Gordon putting job %p at wait list pos %d\n", job, wait_next);
#endif
    wait_list[wait_next] = job;
    wait_callbacks[wait_next].func = callback;
    wait_callbacks[wait_next].arg = callarg;
    wait_next = (wait_next + 1) & (wait_size - 1);
    wait_used++;

}

void gordon_pushjob (gordon_job_t *job, void (*callback)(void *),
                     void *callarg) {
    int status, spe_index;
    unsigned u, min_pending;
    
    ASSERT(job != NULL && ((uintptr_t) job & 0xf) == 0);//job has to be aligned

    mutex_lock(&mutex);
    
    ASSERT(gordon_users > 0);
    
    //find the spe with the lowest # of pending jobs and an available mbox slot
    for (u = 0, min_pending = GORDON_MAX_PENDING, spe_index = -1;
         u < spe_nused; u++) {
        if (spes[spe_next].pending < min_pending) {
            status = spe_in_mbox_status(spes[spe_next].spe_id);
            if (__builtin_expect(status < 0, 0)) {
                int errsv = errno;
                printf("Gordon: Error %d reading SPE %p mailbox status: %s\n",
                       status, spes[spe_next].spe_id, strerror(errno));
                exit(errsv);
            } else if (status > 0) {
                spe_index = spe_next;
                min_pending = spes[u].pending;
            }
        }

        spe_next++;
        if (spe_next == spe_nused) spe_next = 0;
    }

    if (min_pending == GORDON_MAX_PENDING) {
        //all spes are fully busy -> put job onto wait list
        gordon_wait_add(job, callback, callarg);
    } else {
        unsigned int job_index;
        ASSERT(spe_index >= 0);
        spes[spe_index].pending++;
    
        //find free index, it must be available somewhere
        for (u = 0, job_index = busy_next;
             u < busy_size && busy_list[job_index].job != (uintptr_t)NULL;
             u++, job_index = (job_index == busy_size - 1) ? 0 : (job_index + 1));
        ASSERT(u < busy_size);
    
        busy_list[job_index].job = (uintptr_t) job;
        busy_next = (job_index + 1) & (busy_size - 1);
        busy_used++;
        busy_callbacks[job_index].func = callback;
        busy_callbacks[job_index].arg = callarg;

        gordon_submit(spe_index, job_index);
    }
    
    gordon_wait_locked(0); //start polling; handover lock
}

/** This is called when the specified spe has a slot available because
 * the specified job has finished. If there is a job on the waiting list,
 * it is sent to that spe, otherwise the entries are cleaned up
 */
static void gordon_finished (unsigned int spe_index, unsigned int busy_index) {
#ifdef GORDON_DEBUG
    printf("Gordon: Recvd %d from SPU %p, cb func = %p\n", busy_index,
           spes[spe_index].spe_id, busy_callbacks[busy_index].func);
#endif
            
    if (wait_used > 0) {
        unsigned int wait_index = (wait_next - wait_used) & (wait_size - 1);
        busy_list[busy_index].job = (uintptr_t) wait_list[wait_index];
        busy_callbacks[busy_index] = wait_callbacks[wait_index];
        wait_used--;
#ifdef GORDON_DEBUG
        printf("Gordon got job %p from waiting list pos %d, used = %d\n",
               wait_list[wait_index], wait_index, wait_used);
#endif
        gordon_submit(spe_index, busy_index);
    } else {
        spes[spe_index].pending--;
        busy_list[busy_index].job = (uintptr_t) NULL;
        busy_used--;
    }
}
    
/** Wait until n jobs have completed
 * @return 0: no SPU was busy; 1: an SPU might still be busy
 */
int gordon_wait (unsigned int n) {
    mutex_lock(&mutex);
    if (busy_used == 0) {
        ASSERT(wait_used == 0);
        mutex_unlock(&mutex);
        return 0;
    }
    //else
#ifdef DEBUG
    if (n > 1) {
        printf("Gordon waiting for %d jobs with %d busy and %d waiting\n", n,
               busy_used, wait_used);
    }
#endif
    
    if (n > busy_used + wait_used) {
#ifdef DEBUG
        printf("Gordon Warning: waiting for %d jobs while %d are active!\n",
               n, busy_used + wait_used);
#endif
        n = busy_used + wait_used;
    }
    
    gordon_wait_locked(n);
    return 1;
}

/** Poll gordon for completion of its jobs
 * @return same as gordon_wait
 */
int gordon_poll (void) {
    return gordon_wait(0);
}

/** Wait until all pending jobs have finished  */
void gordon_join (void) {
    mutex_lock(&mutex);
    gordon_wait_locked(busy_used + wait_used);
}

void gordon_terminate (void)
{
    unsigned int k;
    int status;
    
    mutex_lock(&mutex);
    ASSERT(gordon_users > 0);
    gordon_users--;
    k = gordon_users;
    mutex_unlock(&mutex);
    
    if (k > 0) return;
    
    ASSERT(busy_used == 0 && wait_used == 0);

    for (k = 0; k < spe_nused; k++) { //send terminate msg to all spe's
        int msg = -1;
        ASSERT(spe_in_mbox_status(spes[k].spe_id) == 4);
        status = spe_in_mbox_write(spes[k].spe_id, (unsigned int *)&msg, 1,
                                   SPE_MBOX_ANY_NONBLOCKING);
        if (__builtin_expect(status != 1, 0)) {
            int errsv = errno;
            printf("Gordon: Couldn't write -1 to input mailbox of SPE %p!\n",
                   spes[k].spe_id);
            printf("Gordon: status = %d, if -1 error msg: %s", status,
                   strerror(errno));
            exit(errsv);
        }
    }
        
    //wait for spe threads. This is separated to allow better timing of the
    //end times of the automatons. Otherwise the automaton of the second spu
    //stays active while the first spe thread joins on the ppu
    for (k = 0; k < spe_nused; k++) { 
        status = pthread_join(spes[k].thread, NULL); ASSERT(status == 0);
    }

#if (GORDON_ACK == GORDON_INTERRUPTS_ONLY)
    status = pthread_join(event_thread, NULL); ASSERT(status == 0);
#endif

    for (k = 0; k < spe_nused; k++) {
        status = spe_context_destroy(spes[k].spe_id); ASSERT(status == 0);
    }
}

void *gordon_malloc (size_t size)
{
    //round size up to a multiple of 16 so we can always dma multiples
    //of 16 bytes
    if (size < 128) return samalloc(roundup(size, 16), 16);
    else return samalloc(roundup(size, 16), 128);
}

gordon_job_t *gordon_alloc_jobs (unsigned int njobs, int reuse_buffers) {
    gordon_job_t *jobs = gordon_malloc(njobs * sizeof (gordon_job_t));
    unsigned int u;
    if (reuse_buffers) reuse_buffers = 1; //else reuse_buffers = 0;
    
    for (u = 0; u < njobs - 1; u++) {
        jobs[u].next = (uintptr_t) &jobs[u+1] | reuse_buffers;
    }
    jobs[u].next = (uintptr_t) NULL;
    return jobs;
}

/** Use this to create a correct striding field */
gordon_strideSize_t gordon_stride_init 
(unsigned int strides, unsigned int size, unsigned int distance) {
    gordon_strideSize_t ss;
    unsigned int gap = distance - size;
    
    ASSERT(distance >= size &&
           strides <= GORDON_MAX_STRIDES &&
           size <= GORDON_MAX_STRIDE_SIZE && (size & 0xf) == 0 &&
           gap <= GORDON_MAX_STRIDE_GAP && (gap & 0xf) == 0);
    
    if (size == 0) {
        ss.size = 0; //no striding, strided bit is automagically set to 0
    } else if (size == distance) {
        ss.size = strides * size; //again, no striding
        //upper/strided bit is set to 0 as the size never needs 32 bits
        ASSERT(ss.stride.strided == 0);
    } else {
        ss.stride.strided = 1;
        ss.stride.strides = strides - 1;
        ss.stride.size = (size >> 4) - 1;
        ss.stride.gap = (gap >> 4) - 1;
    }
    return ss;
}
