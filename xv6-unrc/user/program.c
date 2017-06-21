#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "syscall.h"
int
main(void)
{
  int r,w,fd;
  // int fd;

  fd = open("arch", O_CREATE);
  r = open("arch", O_CREATE);
  w = open("arch", O_CREATE);
  printf(1, "FD FD FD FD FD FD FD %d\n", fd);
  printf(1, "FD FD FD FD FD FD FD %d\n", r);
  printf(1, "FD FD FD FD FD FD FD %d\n", w);

  // r = open("arch", O_RDWR);
  // w = open("arch", O_RDWR);
  // if(fd >= 0) {
  //   printf(1, "ok: create buffer file succeed\n");
  // } else {
  //   printf(1, "error: create buffer file failed 1\n");
  //   exit();}
  // if(r >= 0) {
  //   printf(1, "ok: create buffer file succeed\n");
  // } else {
  //   printf(1, "error: create buffer file failed 1\n");
  //   exit(); }
  // if(w >= 0) {
  //   printf(1, "ok: create buffer file succeed\n");
  // } else {
  //   printf(1, "error: create buffer file failed 2\n");
  //   exit();
  // }
  // int b = 7;
  // int buffer;
  // write(w, &b, 1);  
  // printf(1,"-------------------------- ANTES DEL READ: \n");
  // printf(1,"%d\n", b);  
  // read(r, &buffer, 1);
  // printf(1,"-------------------------- DESPUES DEL READ: %d\n",buffer);
  // close(r);
  // close(w);
  // close(fd);
  exit();
}
