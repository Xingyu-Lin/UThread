/*
 * Created by Xingyu Lin at Jan 21, 2016
 * Student ID: 1300012782
 * */

#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

typedef unsigned int uint;

//User thread control block
static struct utcb
{
    uint tid;
};

uint uthread_create()
{

}
