// Ejemplo de Productor consumidor.
//   
// Recordar: 
// * acceso al contenido
// & obtension de la direccion
// printf(1,"LOG semprod %d\n", semprod);

#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "syscall.h"

#define PRODUCERS 4
#define CONSUMERS 4
#define BUFF_SIZE 4

int buffer;
int semprod;
int semcom;
int sembuff;
int semprueba;
int semprueba2;

void
productor(int fd)
{
  // int fd;
  int n;
  int m;

  printf(1,"-- INICIA PRODUCTOR --\n");
  semdown(semprod); // empty
  semdown(sembuff); // mutex
  //  REGION CRITICA
  //open, read, n++, write, close
  
  fd = open("buffer", O_RDWR);
  read(fd, &n, 1);
  m = n++;
  write(fd, &m, 1);
  close(fd);

  // fscanf(fp, "%d", &n);
  // fprintf(fp, "%d", n++);

  // printf(1,"-- Produce: %d\n",val);
  semup(sembuff); //mutex
  semup(semcom); // full
  printf(1,"-- FIN PRODUCTOR --\n");
}

void
consumidor(int fd)
{ 
  int n;
  int m;
  // int fd;

  printf(1,"-- INICIA CONSUMIDOR --\n");
  semdown(semcom); // full
  semdown(sembuff); // mutex
  // REGION CRITICA
  // fscanf(fp, "%d", &n);
  // fprintf(fp, "%d", n++);
  
  fd = open("buffer", O_RDWR);
  read(fd, &n, 1);
  m = n--;
  write(fd, &m, 1);
  
  close(fd);

  // printf(1,"-- Consume: %d\n",val);
  semup(sembuff); // mutex
  semup(semprod); // empty
  printf(1,"-- FIN CONSUMIDOR --\n");
}

int
main(void)
{
  int fd;
  int val = 8;
  int res;
  int pid_prod, pid_com, i;

  fd = open("buffer", O_CREATE);
  if(fd >= 0) {
    printf(1, "ok: create buffer file succeed\n");
  } else {
    printf(1, "error: create buffer file failed\n");
    exit();
  }

  write(fd, &val, 1);

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
  sembuff = semget(-1,1); 
  if(sembuff < 0){
    printf(1,"invalid sembuff\n");
    exit();
  }

  for (i = 0; i < PRODUCERS; i++) {
    // create producer process
    pid_prod = fork();
    if(pid_prod < 0){
      printf(1,"can't create producer process\n");
      exit(); 
    }
    // launch producer process
    if(pid_prod == 0){ // hijo
      printf(1," # Child producer\n");
      semget(semprod,0);
      semget(semcom,0);
      semget(sembuff,0);
      productor(fd); 
      exit();
    }
  }

  for (i = 0; i < CONSUMERS; i++) { 
    // create consumer process
    pid_com = fork();
    if(pid_com < 0){
      printf(1,"can't create consumer process\n");
      exit(); 
    }
    // launch consumer process
    if(pid_com == 0){ // hijo
      printf(1," # Child consumer\n");
      semget(semprod,0);
      semget(semcom,0);
      semget(sembuff,0);
      consumidor(fd); 
      exit();
    }
  }

  for (i = 0; i < PRODUCERS + CONSUMERS; i++) {
    wait();
  }

  if(read(fd, &res, 1) == val)
    printf(1, "Final result: %s\n", "TRUE");
   
  close(fd);

  exit();
}






