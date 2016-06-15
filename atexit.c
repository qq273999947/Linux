#include<stdio.h>

void func1()
{
    printf("I am func1\n");
}
void func2()
{
    printf("I am func2\n");
}
void func3()
{
    printf("I am func3\n");
}
int main()
{
    atexit(func1);
    atexit(func2);
    atexit(func3);

    puts("this is executed first.");

    return 0;
}
