#include<lpc21xx.h>
#include "header.h"

int calculator_fun(char *);
int priority_check(char );
int solve_equation(char ,int ,int );

int main()
{
	char a[50];
	int result;
	uart0_init(9600);
	while(1)
	{
			uart0_tx_string("Enter the expression\r\n");
			uart0_rx_string(a,50);													//getting input from user
			uart0_tx_string(a);															//loopback
			uart0_tx_string("\r\n");												//new line;
		
			result=calculator_fun(a);												//calling calculator function
			uart0_tx_string("RESULT : ");
			uart0_tx_integer(result);												//printing the result
			uart0_tx_string("\r\n----------------\r\n");
	}

}

//checking the priority of the operators 
int priority_check(char op)
{
if(op=='+' || op=='-')
	return 1;
if(op=='*' || op=='/')
	return 2;


return 0; //if user provides wrong operators
}

//operations on operands 
int solve_equation(char op,int a,int b)
{
	switch(op)
	{
		case '+' : return a+b;
		case '-' : return a-b;
		case '*' : return a*b;
		case '/' : 
							if(b==0)          //check second operand 
							{
								uart0_tx_string("floating point exception\n");
								return 0;
							}
							 return a/b;			
							
		default : return 0;
	}
}

int calculator_fun(char *expression)
{
	int number_stack[100];		//to store number
	int number_top=-1;			//for reference purpose
	char operator_stack[100];	//to store operators
	int operator_top=-1;		//for reference purpose
	int num;
	char *ptr;				
	
	for(ptr=expression ; *ptr!='\0' ; ptr++)
	{
		if(*ptr>='0' && *ptr<='9')
		{
			num=0;
			while(*ptr>='0' && *ptr<='9')
			{
				num=num*10 + (*ptr-'0');
				ptr++;
			}
			ptr--;
			number_stack[++number_top]=num;
		}
		
		else if(*ptr=='+' || *ptr=='-' || *ptr=='*' || *ptr=='/')
		{
			while(operator_top>=0 && priority_check(operator_stack[operator_top])>=priority_check(*ptr))
			{
				number_stack[number_top-1]=solve_equation(operator_stack[operator_top--],number_stack[number_top-1],number_stack[number_top]);
				number_top--;
			}
			operator_stack[++operator_top]=*ptr;
		}
	}
	
	while(operator_top>=0)
	{
		number_stack[number_top-1]=solve_equation(operator_stack[operator_top--],number_stack[number_top-1],number_stack[number_top]);
		number_top--;
	}
	
	return number_stack[0];
}