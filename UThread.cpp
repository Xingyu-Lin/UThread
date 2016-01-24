#include "UThread.h"

// possible status for each thread
#define FREE 0
#define RUNNING 1
#define RUNNABLE 2

//User thread control block
typedef struct uthread_t
{
    uint status;     // can be FREE,RUNNING,RUNNABLE,SUSPEND
    ucontext_t context; // current context
    void* arg;  // arguments for the starting function
    char* stack[DEFAULT_STACK_SIZE]; // thread stack
};

static uthread_t thread_pool[UTHREAD_MAX_THREAD]; // a pool of all threads controlled by the scheduler
static uthread_t thread_sched;  // a special thread for the scheduler
static uint running_thread; // the ID of the currently running thread

uint uthread_self()
{
    return running_thread;
}

static void uthread_wrapper(Fun func, void* arg, uint* status)
{
    func(arg);
    //printf("Thread %u reaped \n",uthread_self());
    *status = FREE;
}

static void uthread_make(uthread_t* uthread)
{
    getcontext(&uthread->context);
    uthread->context.uc_stack.ss_sp = uthread->stack;
    uthread->context.uc_stack.ss_size = DEFAULT_STACK_SIZE;
    uthread->context.uc_stack.ss_flags = 0;
    uthread->context.uc_link = NULL;
    uthread->status = RUNNABLE;
}

int uthread_create(uint* tid, Fun func, void* arg)
{
    uint i;
    for (i=0; i<UTHREAD_MAX_THREAD; ++i)
        if (thread_pool[i].status == FREE)
            break;
    if (i == UTHREAD_MAX_THREAD)
        return -1;  // no currently available threads
    *tid = i;
    printf("thread tid %u created\n", *tid);
    uthread_make(&thread_pool[i]);
    thread_pool[i].context.uc_link = &thread_sched.context;
    thread_pool[i].arg = arg;
    makecontext(&thread_pool[i].context, (void (*)(void)) uthread_wrapper, 3, func, arg, &thread_pool[i].status);
    return 0;
}

// A simple preemptive, round robin scheduler
static void scheduler()
{
    bool thread_exist;
    while (1)
    {
        for (uint i = 0; i < UTHREAD_MAX_THREAD; ++i)
            if (thread_pool[i].status == RUNNABLE)
            {
                thread_pool[i].status = RUNNING;
                running_thread = i;
                swapcontext(&thread_sched.context, &thread_pool[i].context);
            }
        thread_exist = false;   // Check if there is still thread waiting to be scheduled
        for (uint i = 0; i < UTHREAD_MAX_THREAD; ++i)
            if (thread_pool[i].status != FREE)
            {
                thread_exist = true;
                break;
            }
        if (!thread_exist) break; //schedule finished
    }
    printf("scheduler finsihed\n");
}

void uthread_yield()
{
    thread_pool[uthread_self()].status = RUNNABLE;
    swapcontext(&thread_pool[uthread_self()].context, &thread_sched.context);
}

void uthread_join(uint tid)
{
    while (thread_pool[tid].status != FREE)
        uthread_yield();
}

static bool initialized = false;
static itimerval tick;

static void handler(int sig) { uthread_yield(); }

static void uthread_init()  //Set the time alarm and handler
{
    initialized = true;
    signal(SIGALRM, handler);
    memset(&tick, 0, sizeof(tick));
}

bool finish_all= false; // check if all threads have all finished
static ucontext_t ret_context;

void uthread_runall()
{
    if (!initialized) uthread_init();
    finish_all = false;
    getcontext(&ret_context);
    if (finish_all) return;
    finish_all = true;

    tick.it_value.tv_sec = 1;
    tick.it_value.tv_usec = 0;
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = TIME_SLICE;

    int res = setitimer(ITIMER_REAL, &tick, NULL);
    if (res != 0)
    {
        printf("set timer failed\n");
    };

    uthread_make(&thread_sched);
    thread_sched.context.uc_link = &ret_context;
    makecontext(&thread_sched.context, (void (*)(void)) scheduler, 0);

    setcontext(&thread_sched.context);
}
