#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "syscall.h"

int
main(void)
{
  int fd;
  // int n;
  // int val = 8;
  // int pid_prod, pid_com, i;
  // char path[] = "buffer";
  int data[512];

  fd = open("buffer", O_CREATE | O_RDWR);
  if(fd >= 0) {
    printf(1, "ok: create buffer file succeed\n");
  } else {
    printf(1, "error: create buffer file failed\n");
    exit();
  }

  int t = 100;
  write(fd, data, sizeof t);
  
  read(fd, data, sizeof data);


  //Como se imprime contenido de data??
  printf(1,"-------------------------- VALOR INICIAL: [%d] \n", data[1]);
  // printf(1,"--- Tamano de buffer: %d\n", BUFF_SIZE);
  
  close(fd);
  exit();
  // printf(1,"------------------------- VALOR FINAL: [%x]  \n", val);
}
