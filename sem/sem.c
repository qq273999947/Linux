#include"common.h"

static int com_sem(int nsems,int flag)
{
    key_t key = ftok(_PATH_,_PROJ_ID_);
    if(key < 0){
        printf("%s\n",strerror(errno));
        return -1;
    }
    int sem_id = semget(key,nsems,flag);
    if(sem_id < 0){
        printf("%s\n",strerror(errno));
        return -2;
    }
    return sem_id;
}

int creat_sem(int nsems)
{
    int flag = IPC_CREAT|IPC_EXCL|0666;
    return com_sem(nsems,flag);
}

int get_sem(int nsems)
{
    int flag = IPC_CREAT;
    return com_sem(nsems,flag);
}

static int com_semop(int sem_id,unsigned short num,short op)
{
    struct sembuf semb[1];//only consider binary semaphore
    semb[0].sem_num =num;
    semb[0].sem_op = op;
    semb[0].sem_flg = 0;//SEM_UNDO,对崩溃的信号量进行回滚操作
    if(semop(sem_id,semb,1) < 0)
    {
        printf("%s\n",strerror(errno));
        return -1;
    }
    return 0;
}

int P(int sem_id,unsigned short num)
{
    int op = -1;
    return com_semop(sem_id,num,op);
}

int V(int sem_id,unsigned short num)
{
    int op = 1;
    return com_semop(sem_id,num,op);
}

int init_sem(int sem_id,int num,int init_val)
{
    semun_t sem_val;
    sem_val.val = init_val;
    int ret = semctl(sem_id,num,SETVAL,sem_val);
    if(ret < 0){
        printf("%s\n",strerror(errno));
        return -1;       
    }
    return 0;
}
int destory_sem(int sem_id)
{
    int ret = semctl(sem_id,0,IPC_RMID);//0是指定的信号量，此处讨论的是二元信号量
    if(ret < 0){
        printf("%s\n",strerror(errno));
        return -1;    
    }
        printf("\ndestroy success!\n");
        return 0;
}
