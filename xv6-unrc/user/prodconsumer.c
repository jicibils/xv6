#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "syscall.h"

#define PRODUCERS 4
#define CONSUMERS 2
#define BUFF_SIZE 4
// #define N  4

int buffer;
int semprod;
int semcom;
int sembuff;
int semprueba;
int semprueba2;

void
producer(int val)
{
  printf(1,"-- START PRODUCER --\n");
  semdown(semprod); // empty
  semdown(sembuff); // mutex
  //  Critical region
  val = (val) + 1;
  printf(1,"-- Produce: %d\n",val);
  semup(sembuff); //mutex
  semup(semcom); // full
  printf(1,"-- END PRODUCER --\n");
}

void
consumer(int val)
{ 
  printf(1,"-- START CONSUMER --\n");
  semdown(semcom); // full
  semdown(sembuff); // mutex
  // Critical region
  val = (val) - 1;
  printf(1,"-- Consume: %d\n",val);
  semup(sembuff); // mutex
  semup(semprod); // empty
  printf(1,"-- END CONSUMER --\n");
}

int
main(void)
{
  int val = 8; 
  int pid_prod, pid_com, i;

  printf(1,"-------------------------- INIT VALUE: [%d] \n", val);
  printf(1,"-------------------------- BUFFER SIZE: %d\n", BUFF_SIZE);
  
  // creo semaforo productor 
  semprod = semget(-1,BUFF_SIZE); // empty
  // semprod es cero
  if(semprod < 0){
    printf(1,"invalid semprod \n");
    exit();
  }
  // creo semaforo consumidor
  semcom = semget(-1,0); // full   
  if(semcom < 0){
    printf(1,"invalid semcom\n");
    exit();
  }
  // creo semaforo buffer
  sembuff = semget(-1,1); // mutex 
  if(sembuff < 0){
    printf(1,"invalid sembuff\n");
    exit();
  }

  for (i = 0; i < PRODUCERS; i++) { // 4 productores 
    // create producer process
    pid_prod = fork();
    if(pid_prod < 0){
      printf(1,"Can't create producer process\n");
      exit(); 
    }
    // launch producer process
    if(pid_prod == 0){ // hijo
      printf(1," # child producer\n");
      semget(semprod,0);
      semget(semcom,0);
      semget(sembuff,0);
      producer(val); 
      exit();
    }
  }

  for (i = 0; i < CONSUMERS; i++) { // 2 consumidores 
    // create consumer process
    pid_com = fork();
    if(pid_com < 0){
      printf(1,"can't create consumer process\n");
      exit(); 
    }
    // launch consumer process
    if(pid_com == 0){ // hijo
      printf(1," # child consumer\n");
      semget(semprod,0);
      semget(semcom,0);
      semget(sembuff,0);
      consumer(val); 
      exit();
    }
  }

  for (i = 0; i < PRODUCERS + CONSUMERS; i++) {
    wait();
  }
   
  printf(1,"-------------------------- FINAL VALUE OF VAL: %x  \n", val);
  exit();
}






