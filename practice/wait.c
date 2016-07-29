#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include<errno.h>
#define _PORC_NUM_ 10
#define _DEFAULT_PID_ -1

int child_run()
{
    srand(time(NULL));
    int _time = rand()%30;
    printf("this child pid :%d,sleep time is :%d \n",getpid(),_time);
    sleep(_time);
    return 0;
}

int creat_proc(pid_t *pid_l,int num)
{
    if(pid_l != NULL && num > 0){
        int i = 0;
        for(;i < num;++i){
            pid_t id = fork();
            if(id < 0)
            {
                printf("%s:creat %d proc failed\n",errno,i);
                return 1;
            }
            else if(id == 0){
                //child
                int child_ret = child_run();
                exit(1);
            }
            else{
                pid_l[i] = id;
            }
        }
    }
    return 0;
}

int wait_proc(pid_t *pid_l,int num)
{
    int wait_ret = 0;
    if(pid_l != NULL && num > 0){
        int i = 0;
        for(;i < num;++i){
            if(pid_l[i] == _DEFAULT_PID_){
                continue;
            }
            int status = 0;
            int ret = waitpid(pid_l[i],&status,0);
            if(WIFEXITED(status) && ret == pid_l[i]){
                printf("wait child pid :%d,success,return code is %d\n",pid_l[i],WEXITSTATUS(status));
            }
             else{
                    printf("wait child pid :%d,failed\n",pid_l[i]);
                    wait_ret = 1;
            }
        }
    }
    return wait_ret;
}

int main()
{
    pid_t _pid_list[_PORC_NUM_]; 
    int i = 0;
    for(; i<_PORC_NUM_; i++){
        _pid_list[i] = _DEFAULT_PID_;
    }
    if( creat_proc(_pid_list, sizeof(_pid_list)/sizeof(_pid_list[0])) == 0  ){
        printf("%s :create all proc success!\n",__FUNCTION__);
    }
    else{    
        printf("%s :not all proc create success!\n",__FUNCTION__);
    }
    if( wait_proc(_pid_list, sizeof(_pid_list)/sizeof(_pid_list[0])) == 0  ){
        printf("%s :wait all proc success!\n",__FUNCTION__);     

    }else{   
        printf("%s :not wait all proc success!\n",__FUNCTION__);      
    }
    return 0;
}
