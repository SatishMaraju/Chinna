#include <pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>

char p[]="hai";
int n=0;
void *thread_fun(void *arg);
int main()
{
	int b;
	void *retval;
	pthread_t abc;   // like a file descriptor
	int ret=pthread_create(&abc,NULL,thread_fun,(void *) p);
	if(ret<0)
	{
	perror("message:");
	}
	ret=pthread_join(abc,&retval);  // like function calling
	if(ret<0)
	{
	perror("message:");
	}
	n=0;  //initialise to zero after completion of sub function otherwise fail condition
	while((n>=0)&&(n<=100))
	{
		if(n%2==0){
			printf("even:%d\n",n);++n;
		sleep(1);}
		else;++n;
		
	}
sleep(5);
}

void *thread_fun(void *arg)
{

	while((n>=0)&&(n<=100))
	{
		if(n%2!=0){
			printf("odd:%d\n",n);++n;
		sleep(1);}
		else;
	n++;
	}
	pthread_exit("hello chinna");
	sleep(5);
}

