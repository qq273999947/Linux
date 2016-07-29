#include"common.h"

int main()
{
    int size = 1024;
    int shm_id = creat_shm(size);
    sleep(10);
    char* mem = (char*)attach(shm_id);
    memset(mem,'\0',size);

    int count = 10;
    while(count--){
        printf("%s\n",mem);
        sleep(1);
    }

    detach(mem);
    destory_shm(shm_id);
    return 0;
}
