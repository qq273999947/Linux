#include "common.h"

int main()
{
        int f_sem_id = creat_sem(1);
        init_sem(f_sem_id,0,1);
        pid_t id = fork();
        if( id == 0 ){//child
            int c_sem_id = get_sem(1);
            int count = 3;
            while(1){
                P(c_sem_id,0);
                printf("A");
                fflush(stdout);
                sleep(3);
                printf("A");
                fflush(stdout);
                sleep(3);
                V(c_sem_id,0);
                count--;
                if(count == 0){
                     break;             
                 }
            }
        }
        else
        {//father
            int count = 3;
             while(1) {
               P(f_sem_id,0);
               printf("B");
               fflush(stdout);
               sleep(3;
               printf("B");
               fflush(stdout);
               sleep(2;
               V(f_sem_id,0);
               count--;
               if(count == 0){
                  break;  
                }
                    
            }
        }
}
