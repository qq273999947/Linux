#pragma once 

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<errno.h>
#include<string.h>

#define _PATH_ "."
#define _PROJ_ID_ 0x6666

int creat_shm(int size);
int get_shm(int size);
int destory_shm(int shm_id);
void* attach(int shm_id);
int detach();
