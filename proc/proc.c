#include<stdio.h>
#include<string.h>
void porc()
{
	int rate = 0;
	char bar[102];
	char index[5] = "-\\|/\0";
	memset(bar,'\0',sizeof(bar));
	while(rate <= 100){
		bar[rate] = '=';
		printf("[%-101s][%d%%][%c]\r",bar,rate,index[rate%4]);
		fflush(stdout);
		rate++;
		usleep(100000);
	}
}
int main()
{
	porc();
	return 0;
}
