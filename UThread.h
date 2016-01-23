/*
 * Created by Xingyu Lin at Jan 21, 2016
 * Student ID: 1300012782
 * */

#ifndef UTHREAD_H_INCLUDED
#define UTHREAD_H_INCLUDED
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

#define UTHREAD_MAX_THREAD 100

#define UTHREAD_STATUS_FREE  	        0
#define UTHREAD_STATUS_RUNNING 	        1
#define UTHREAD_STATUS_RUNNALBE 	    2
#define UTHREAD_STATUS_SUSPEND 		    3

#define DEFAULT_STACK_SIZE 2048

typedef unsigned int uint;

typedef void (*Fun) (void* arg);

// initialize for user thread library
void uthread_init();

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

#endif