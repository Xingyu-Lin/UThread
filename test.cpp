//
// Created by Xingyu Lin on 1/21/16.
//

#include "UThread.h"
#include <stdio.h>

void print(void* arg)
{
    while (1)
    {
        printf("Hello from thread number %u\n", uthread_self());
        sleep(1);
        break;
    }
}

int main()
{
    for (int i=0; i<5; ++i)
    {
        uint tid;
        uthread_create(&tid, print, NULL);
    }
    uthread_runall();
    printf("Main: first round finished\n");
}
