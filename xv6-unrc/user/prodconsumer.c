#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "syscall.h"

#define PRODUCERS 4
#define CONSUMERS 2
#define BUFF_SIZE 4
#define N  4

int buffer;
int semprod;
int semcom;
int sembuff;
int semprueba;
int semprueba2;

void
productor(int val)
{
  printf(1,"-- INICIA PRODUCTOR --\n");
  semdown(semprod); // empty
  semdown(sembuff); // mutex
  //  REGION CRITICA
  val = (val) + 1;
  printf(1,"-- Produce: %d\n",val);
  semup(sembuff); //mutex
  semup(semcom); // full
  printf(1,"-- FIN PRODUCTOR --\n");
}

void
consumidor(int val)
{ 
  printf(1,"-- INICIA CONSUMIDOR --\n");
  semdown(semcom); // full
  semdown(sembuff); // mutex
  // REGION CRITICA
  val = (val) - 1;
  printf(1,"-- Consume: %d\n",val);
  semup(sembuff); // mutex
  semup(semprod); // empty
  printf(1,"-- FIN CONSUMIDOR --\n");
}

int
main(void)
{
  int val = 8; 
  int pid_prod, pid_com, i;

  printf(1,"-------------------------- INIT VALUE: [%d] \n", val);
  printf(1,"--- Tamano de buffer: %d\n", BUFF_SIZE);
  
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
      printf(1,"can't create producer process\n");
      exit(); 
    }
    // launch producer process
    if(pid_prod == 0){ // hijo
      printf(1," # hijo productor\n");
      semget(semprod,0);
      semget(semcom,0);
      semget(sembuff,0);
      productor(val); 
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
      printf(1," # hijo consumidor\n");
      semget(semprod,0);
      semget(semcom,0);
      semget(sembuff,0);
      consumidor(val); 
      exit();
    }
  }

  for (i = 0; i < PRODUCERS + CONSUMERS; i++) { // 6 
    wait();
  }
   
  printf(1,"-------------------------- FINAL VALUE: [%x]  \n", val);
  exit();
}






