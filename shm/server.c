#include"common.h"

int main()
{
    int size = 1000;
    int shm_id = get_shm(size);
    sleep(5);
    char* mem = (char*)attach(shm_id);
    memset(mem,'\0',size);
    int index = 0;
    int count = 10;
    while(count--){
        mem[index++] = 'A';
        mem[index] = '\0';
        sleep(1);
    }
    detach(mem);
    return 0;
}
