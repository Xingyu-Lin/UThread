//
// Created by Xingyu Lin on 1/21/16.
//

#include "UThread.h"
#include <stdio.h>

void print(void* arg)
{
    printf("Hello from thread number %u\n", uthread_self());
}

int main()
{
    for (int i=0; i<10; ++i)
    {
        uint tid;
        uthread_create(&tid, print, NULL);
    }
    uthread_runall();
    return 0;
}
