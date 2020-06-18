#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
int j,i;

//señal 2  ->
//señal 14 ->
void handlerint();
void handleralarm();

void handleralarm(){
	printf("i=%d , j=%d \n",i,j);
	fflush(stdout);
	alarm(1);
}

void handlerint(){
	signal(SIGALRM, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	char c;
	printf("seguro que quiere salir ?");
	fflush(stdout);

	scanf("%c", &c);
	if(c=='y'){
		raise(SIGTERM);
	}
}

void main(void){
	signal(SIGINT, handlerint);
	signal(SIGALRM, handleralarm);
	alarm(1);
	int sum;
	for(i=1000000;i>0;i--){
		for(j=1000000;j>0;j--){
			sum += i +j;
		}
	}
}
