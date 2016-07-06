#pragma once

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<errno.h>
#include<string.h>

#define _PATH_ "."
#define _PROJ_ID_ 0x6666

typedef union semum{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
}semun_t;

int creat_sem(int nsems);
int get_sem(int nsems);
int init_sem(int sem_id,int num,int init_val);
int P(int sem_id,unsigned short num);
int V(int sem_id,unsigned short num);
int destory_sem(int sem_id);
