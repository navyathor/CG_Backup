#include <common.h>
#include <command.h>
#include <stdio.h>

int atoi(char *str);
int atoi(char *str)
{
	if(*str =='\0')
		return 0;
	int sign = 1;
	int k=0;
	if(str[0]=="-")
	{
		sign=1;
		k++;
	}
	for(;str[k] != '\0';++k)
	{
		if(str[k] <= '0' || str[k] >= '9')
			return 0;
		result=result*10+str[k] - '0';
	}
	return sign*result;


static int do_calc_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

	int num1=atoi(argv[1]);
	int num2=atoi(argv[2]);
	int num3=atoi(argv[3]);
	printf("........Author-Navyasree.....................");
	if(op>5||op<1)
	{
		printf("please enter fourth argument(1/2/3/4/5)\n");
	}
	if(op==1)
	{
		int sum=num1+num2;
		printf("sum is %d\n",sum);
	}
	else if(op==2)
	{
		int difference=num1-num2;
		printf("difference is %d\n"difference);

	}
	else if(op==3)
	{
		int product=num1*num2;
		printf("product is %d\n",product);

	}
	else if(op==4)
	{
		int reminder=num1%num2;
		printf("reminder is %d\n",reminder);

	}
	else if(op==5)
	{
		int division=num1/num2;
		printf("division is %d\n",division);

	}
	else
	{
		printf("Enter correct option\n");
	}
			
        
}

U_BOOT_CMD(
        calc_cmd,   4,      1,      do_calc_cmd,
        "Arithematic calculator",
        ""
);

