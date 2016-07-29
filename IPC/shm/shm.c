#include"common.h"

static int comm_shm(int size,int flag)
{
    key_t key = ftok(_PATH_,_PROJ_ID_);
    if(key == -1){
        printf("%s\n",strerror(errno));
        return -1;
    }
    int shm_id = shmget(key,size,flag);
    if(shm_id == -1){
        printf("%s\n",strerror(errno));
        return -2;
    }
    return shm_id;
}

int creat_shm(int size)
{
    int flag = IPC_CREAT|IPC_EXCL|0666;
    return comm_shm(size,flag);
}

int get_shm(int size)
{
    int flag = IPC_CREAT;
    return comm_shm(size,flag);

}

int destory_shm(int shm_id)
{
    if(shmctl(shm_id,IPC_RMID,0) == -1){
        printf("%s\n",strerror(errno));
        return -1;
    }
    return 0;
}

void* attach(int shm_id)
{
    void* mem = NULL;
    if(mem = shmat(shm_id,NULL,0)){
        return mem;
    }
    return NULL;
}

int detach(void * addr)
{
    return shmdt(addr);
}
