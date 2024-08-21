#include<LPC21xx.H>
#include "header.h"
/*
void uart0_Handler(void) __irq
{
	static int i;
	int r=U0IIR;
	r=r>>1&7;
	if(r==2)
	{
	a[i]=U0RBR;
		i++;
		if(a[i-1]=='\r')
		{
		a[i-1]='\0';
			i=0;
			flag=1;
		}
		if(i==12)
		{
		a[i]='\0';
			i=0;
			flag=1;
		}
	}
	VICVectAddr=0;
}


void config_vic_for_uart0(void)
{
	VICIntSelect=0;
	VICVectCntl0=6|(1<<5);
	VICVectAddr0=(unsigned int) uart0_Handler;
	VICIntEnable=(1<<6);
}
*/

void en_uart0_interrupt(void)
{
U0IER=3;
}

void uart0_init(u32 baud)
{
int a[]={15,60,30,15,15};
u32 result=0;
u32 pclk=a[VPBDIV]*1000000;
result=pclk/(16*baud);
PINSEL0 |=0x5;
U0LCR=0x83;
U0DLL=result & 0xff;
U0DLM=(result>>8)& 0xff;
U0LCR=0x03;
}

#define THRE ((U0LSR>>5)&1)
void uart0_tx(u8 data)
{
U0THR=data;
	while(THRE==0);
}


#define RDR (U0LSR&1) 
u8 uart0_rx(void)
{
while(RDR==0)
{
	;
}

	return U0RBR;
}

void uart0_tx_string(char *ptr)
{
	while(*ptr)
	{
		uart0_tx(*ptr);
		ptr++;
	}
}

void uart0_rx_string(char *p,int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		while(RDR==0);
		p[i]=U0RBR;
		uart0_tx(p[i]);
		if(p[i]=='\r')
			break;
	}
	p[i]='\0';

}


void uart0_tx_integer(int num)
{
if(num<0)
{
	num=-num;
	uart0_tx('-');
}
char a[10];
int i=0;
while(num)
{
	a[i++]=num%10+48;
	num/=10;
}
for(--i;i>=0;i--)
uart0_tx(a[i]);
}