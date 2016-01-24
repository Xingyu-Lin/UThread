//
// Created by Xingyu Lin on 1/21/16.
//

#include "UThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print(void* arg)
{
    int round = 3;
    while (round--)
    {
        printf("Hello from thread number %u\n", uthread_self());
        sleep(2);
    }
}

/*
 * Keeps print hello to see the RR scheduler is working properly
 * */
void test1()
{
    printf("test1 started\n");
    for (int i=0; i<5; ++i)
    {
        uint tid;
        uthread_create(&tid, print, NULL);
    }
    uthread_runall();
    printf("test1 finished\n");
}

uint question[5], answer[5];
void ask(void* arg)
{
    int work = (rand() %50) * 10000000;
    double temp = 1.1;
    for (int i=0; i<work; ++i)
        temp = temp + 0.01;
    printf("Thread %u: How are you?\n", uthread_self());
}

void respond(void* arg)
{
    uthread_join(question[*(uint*) arg]);
    printf("Answer for thread %u: I am fine, thank you and you?\n", question[*(uint*) arg]);
}

/*
 * Mainly for testing of the thread_join function and preemptive scheduling
 * */
void test2()
{
    printf("test2 started\n");
    srand(time(0));
    //5 responding threads
    uint tid;
    for (uint i=0; i<5; ++i)
    {
        answer[i] = i;
        uthread_create(&tid, respond, &answer[i]);
    }
    //5 requesting threads
    for (uint i=0; i<5; ++i)
        uthread_create(&question[i], ask, NULL);
    uthread_runall();
    printf("test2 finished\n");
}

int main()
{
    freopen("test2.out", "w", stdout);
    //test1();
    test2();
}
