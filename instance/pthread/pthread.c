#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>

void *thread_run(void* arg)
{
  //  int ret = pthread_detach(pthread_self());//线程分离 
  //  if(ret != 0){
  //      printf("%d : %s\n",ret,strerror(ret));
  //      return 1;
  //  }

    int val = (int)arg;
    int count = 0;
    while(1){
        printf("this is new thread,ard:%d,ID:%lu\n",val,pthread_self());
        sleep(1);
        count++;
        if(count > 5){
            printf("new thread done..\n");
            //return (void*)0;//终止方法1：从线程函数中return 
            //pthread_exit((void*)66);//终止方法2：自行终止
        }
    }

}

int main()
{
    pthread_t id;
    int ret = pthread_create(&id,NULL,thread_run,(void*)1);
    if(ret != 0){
        printf("%d : %s\n",ret,strerror(ret));
        return 1;
    }
    int count = 0;
    while(1){
        printf("this is main thread,id:%lu\n",pthread_self());
        sleep(1);
        count++;
        if(count > 10){
            break;
        }else if(count > 5){
            pthread_cancel(id);//终止方法3：主（其他）线程终止
        }
    }

    void* status = NULL;
    ret = pthread_join(id,&status);// 线程等待
    if(ret == 0){
        printf("pthread_join success!\n");
    }else{
        printf("%d:%s\n",ret,strerror(ret));
    }
    return 0;
}
