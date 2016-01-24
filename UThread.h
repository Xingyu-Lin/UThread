/*
 * Created by Xingyu Lin at Jan 21, 2016
 * Student ID: 1300012782
 * */

#ifndef UTHREAD_H_INCLUDED
#define UTHREAD_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define UTHREAD_MAX_THREAD 100
#define DEFAULT_STACK_SIZE 2048

//Length of the time slice for the RR scheduler, in us
#define TIME_SLICE 500000

typedef unsigned int uint;

typedef void (*Fun) (void* arg);

//Create a thread.
//tid: return id for the thread, scheduler: assigned scheduler,
//func: starting function, arg: arguments
//Return 0 on success, -1 otherwise
int uthread_create(uint* tid, Fun func, void* arg);


void uthread_yield();

//start to run all the threads
void uthread_runall();

//return the ID of the currently running thread
uint uthread_self();

//yield the currently running thread
//if the scheduler is running, nothing will happen
void uthread_yield();

//suspend the currently running thread until the thread numbered tid finishes
void uthread_join(uint tid);
#endif