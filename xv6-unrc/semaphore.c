#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "spinlock.h"
#include "stat.h"
#include "proc.h"
#include "semaphore.h"



struct {
  struct spinlock lock;
  struct sem sem[MAXSEM];
} stable;

#define validsem(id) ( id >= 0 && id < MAXSEM )


void
seminit(void)
{
  initlock(&stable.lock, "stable");
}


//show the proc's table of proc
void
showtableproc()
{
  int i;
	cprintf("SHOWTABLEPROC\n");
	for (i=0; i<MAXSEMPROC && proc->sem[i]!= 0; i++){
		cprintf("proc: %d  in pos: %d \n",proc->sem[i],i);
	}   

}

// Given a pointer returns a key of this pointer in the stable
int
getkey(struct sem *s)
{
	if(s < stable.sem || s > &(stable.sem[MAXSEM]))   
		panic("sem key");

	return s-stable.sem;
}



// Given a id returns a semaphore(pointer)
struct sem* 
getstable()
{
	return stable.sem;
}
//return 0 (true) if there is a semaphore available in the system
// else return -1 (false) if there isn't
static int
allocsem(void)
{
	int i;

	for (i=0; i<MAXSEM && stable.sem[i].refcount != 0; i++)
		;
	return (i == MAXSEM)? -1 : i;
}


//return 0 (true) if the process don't have a maximun number of semaphores
// else return -1 (false) if the process have it
static int
freeslot(void)
{
	int i;

	for (i=0; i<MAXSEMPROC && proc->sem[i]!= 0; i++)
		;
	return (i == MAXSEMPROC)? -1 : i;
}

// create a new semaphore and return his id
static int
createsem(struct sem *s,int initvalue,int indexsemproc)
{
  s->refcount = 1;
  s->value = initvalue;
  // proc->semnumber++;
  proc->sem[indexsemproc] = s;
  return 0;
}

//return 0 (true) if the semaphore is in use
// else return -1 (false) if the semaphore isn't in use
static int
seminuse(int semid)
{
  if(stable.sem[semid].refcount == 0)
  	return -1; //false, the semaphore isn't in use

	return 0; //true, the semaphore is in use  
}

//make or took a semaphore's descriptor
//returns the new semaphore or old semaphore
//or in otherwise returns negatives numbers for error
//-1 if the semaphore is not in use 
//-2 the process have a maximun number of semaphores
//-3 the process have a maximun number of semaphores and 
// there isn't other semaphore in the system
int 
semget(int semid, int initvalue)
{

  struct sem *s;

  if(semid < -1 || semid >= MAXSEM){
		panic("semget");
		return -4;//Error
	}

	acquire(&stable.lock);
  
  //the semaphore isn't in use
	if(semid != -1 && seminuse(semid)==-1){
		release(&stable.lock);
		return -1;//Error
	}

  //the semaphore isn't in use
  s = stable.sem + semid;
	if(semid != -1){
		s->refcount++;
		release(&stable.lock);
		return semid;
	}

	//semid == -1 (create new sem)
 	int indexsem = allocsem();

 	if(indexsem == -1){
    release(&stable.lock);
   	return -3;//Error
  }

  //there is a semaphore available in the system
  int indexsemproc = freeslot();

  if(indexsemproc == -1){
    release(&stable.lock);
   	return -2;//Error
  }

	//the process don't have a maximun number of semaphores
 	s = stable.sem + indexsem;
 	createsem(s,initvalue,indexsemproc);
  release(&stable.lock);
  return indexsem;

}



// search the index in the arraysem of proc 
static int 
idsemproc(int semid)
{
	int i;
	int idproc = -1;
	//search in all arraysem of proc
  for (i=0; i<MAXSEMPROC ; i++){
  	if(proc->sem[i]!= 0)//there is a sem in arraysem
			idproc = getkey(proc->sem[i]);

		if(idproc == semid)
			return idproc;
	}
	return -1;
}

// free the semaphore
// returns -1 for error (semaphore didn't get for the process)
// returns 0 in otherwise
int 
semfree(int semid)
{
  struct sem *s;
  int idproc;

  if ( !validsem(semid) )
  	return -1;
	
	acquire(&stable.lock);
  s = stable.sem + semid;
  
	if(seminuse(semid)==-1){//As it is not in use, it can not be released.Error!
		release(&stable.lock);
		return -1;
	}

	idproc = idsemproc(semid);
	if (idproc == -1) {
			release(&stable.lock);
			return -1;
	}
	// OK
	s->refcount--;//Decrement the refcounter because I release the semaphore
	proc->sem[idproc] = 0; //release the semaphore in arraysem of proc	  			
		
	release(&stable.lock);
	return 0;
}


// decrements a unit of value to semaphore 
// and block the process if the value of semaphore is <0
// returns -1 for error (semaphore didn't get for the process)
// returns 0 in otherwise
int 
semdown(int semid)
{
  struct sem *s;

  if ( !validsem(semid) )
  	return -1;

	acquire(&stable.lock);
  s = stable.sem + semid;

	if(seminuse(semid) == -1){//As it is not in use, it can not be get.Error!
		release(&stable.lock);
		return -1;
	}

	while(stable.sem[semid].value == 0)//if the value is zero sleep the process and block 
		sleep(s,&stable.lock);

	stable.sem[semid].value--;
	release(&stable.lock);
	return 0;
}


// increments a unit of value to semaphore 
// and eventually wake up some process of the queue
// returns -1 for error (semaphore didn't get for the process)
// returns 0 in otherwise
int 
semup(int semid)
{
  struct sem *s;

  if ( !validsem(semid) )
  	return -1;

	acquire(&stable.lock);
  s = stable.sem + semid;

	if(seminuse(semid) == -1){//As it is not in use, it can not be get.Error!
		release(&stable.lock);
		return -1;
	}

	if (stable.sem[semid].value >= 0) {
		s->value++;
		if(stable.sem[semid].value == 1)//if the value is zero increment and wakeup 
			wakeup(s);
	}
	release(&stable.lock);
	return 0;
}
