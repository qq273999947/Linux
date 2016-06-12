#include<stdio.h>
#include<stdlib.h>
int g_val = 100;

void test()//stack test
{
    int a = 10;
    int b = 10;
    printf("stack a address:0x%x\n",&a);
    printf("stack b address:0x%x\n",&a);
}
void(*fp)();
int main()
{
    test();
    int c = 10;
    int *heap = malloc(sizeof(int));
    fp = test;
    printf("code address : 0x%x\n",fp);
    printf("data address: 0x%x\n",&g_val);
    printf("heap address : 0x%x\n",heap);
    printf("main stack address: 0x%x\n",&c);
    return 0;
}
