#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFF_SIZE 4

int sem1, sem2, sem3, sem4;

//Test0: creation of more semaphores than allowed per process
void 
testcase0(void)
{

	sem1 = semget(-1, 3);
	printf(1, "sem1 Semaphore id: %d\n", sem1);

	if (sem1 < 0)
	{
		//Aca ya salta error -2. No deberia si quiero crear el 1er sem recien
		printf(1, "Error in sem1\n", sem1);
		exit();	
	} 

	sem2 = semget(-1, 4);
	printf(1, "sem2 Semaphore id: %d\n", sem2);

	if (sem2 < 0)
	{
		printf(1, "Error in sem2\n", sem2);
		exit();	
	} 

	sem3 = semget(-1, 10);
	printf(1, "sem3 Semaphore id: %d\n", sem3);

	if (sem3 < 0)
	{
		printf(1, "Error\n", sem3);
		exit();	
	}

	sem4 = semget(-1, 0);
	printf(1, "sem4 Semaphore id: %d\n", sem4);

	if (sem4 < 0)
	{
		printf(1, "Error. Cannot create any new semaphores: the semaphore limit per process has been reached.\n", sem4);
		exit();	
	}
}

//Create and get a same semaphore 
void 
testcase1(void)
{
	sem1 = semget(-1, 0);
	printf(1, "Just created semaphore with identifier: %d\n", sem1);

	if(sem1 < 0)
	{
		printf(1, "Error\n", sem1);
		exit();
	}
	printf(1, "semaphore created : %d\n", sem1);

	int getsem;
	getsem = semget(sem1,0);  //Ask for semaphore with 5 as identifier but this id doesn't exist because there's only one
	if (getsem == -1)
	{
		printf(1, "Error: It doesn't exist a semaphore with that id\n", getsem);
		exit();
	}
	printf(1, "semaphore obtained : %d\n", getsem);
	exit();
}


//Create and get a other semaphore 
void 
testcase2(void)
{
	sem1 = semget(-1, 0);
	printf(1, "Just created semaphore with identifier: %d\n", sem1);

	if(sem1 < 0)
	{
		printf(1, "Error\n", sem1);
		exit();
	}
	printf(1, "semaphore created : %d\n", sem1);

	int getsem;
	int idsem = 2;
	getsem = semget(idsem,0);  //Ask for semaphore with 5 as identifier but this id doesn't exist because there's only one
	if (getsem == -1)
	{
		printf(1, "Error: It doesn't exist a semaphore with that id %d\n", idsem);
		exit();
	}
	printf(1, "semaphore obtained : %d\n", getsem);
	exit();
}

//Create and make semup and show de value, before and after of modification
void 
testcase3(void)
{
	// int resultsemup;	
	// struct sem *s;

	sem1 = semget(-1, 0);
	printf(1, "Just created semaphore with identifier: %d\n", sem1);

	if(sem1 < 0)
	{
		printf(1, "Error\n", sem1);
		exit();
	}

	// s = getstable() + sem1;

	printf(1, "semaphore created : %d\n", sem1);
	// printf(1, "semaphore's value : %d\n", s.value);

	if (semup(sem1) == 0){

		printf(1, "semaphore created : %d\n", sem1);
		// printf(1, "semaphore's value : %d\n", s.value);
		exit();
	}

	printf(1, "Error: the semaphore's value is wrong \n");
	exit();
}

//Create and make semdown and show de value, before and after of modification
void 
testcase4(void)
{
	// int resultsemup;	
	// struct sem *s;

	sem1 = semget(-1, 0);
	printf(1, "Just created semaphore with identifier: %d\n", sem1);

	if(sem1 < 0)
	{
		printf(1, "Error\n", sem1);
		exit();
	}

	// s = getstable() + sem1;

	printf(1, "semaphore created : %d\n", sem1);
	// printf(1, "semaphore's value : %d\n", s.value);

	if (semdown(sem1) == 0){

		printf(1, "semaphore created : %d\n", sem1);
		// printf(1, "semaphore's value : %d\n", s.value);
		exit();
	}

	printf(1, "Error: the semaphore's value is wrong \n");
	exit();
}

//Create and meke semfree 
void 
testcase5(void)
{
	sem1 = semget(-1, 0);
	printf(1, "Just created semaphore with identifier: %d\n", sem1);

	if(sem1 < 0)
	{
		printf(1, "Error\n", sem1);
		exit();
	}
	printf(1, "semaphore created : %d\n", sem1);

	if (semfree(sem1) == 0){

		printf(1, "semaphore free : %d\n", sem1);
		// printf(1, "semaphore's value : %d\n", s.value);
		exit();
	}

	printf(1, "Error: the semaphore's value is wrong \n");
	exit();
}

int
main(void)
{
	// testcase0();//OK
	// testcase1();//OK
	// testcase2();//OK
	// testcase3();
	// testcase4();
	testcase5();
	exit();
}
