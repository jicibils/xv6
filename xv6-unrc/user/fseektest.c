#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "syscall.h"

int
main(void)
{
  int fd0,fd1,off0,off1;

  fd0 = open("arch0", O_CREATE);
  fd1 = open("arch1", O_CREATE);
  printf(1, "FD0 FD0 FD0 FD0 FD0 FD0 FD0 %d\n", fd0);
  printf(1, "FD1 FD1 FD1 FD1 FD1 FD1 FD1 %d\n", fd1);

  off0 = fgetoff(fd0);
  printf(1, "OFFSET  FD0 FD0 FD0 FD0 FD0 FD0 FD0 %d\n", off0);
  off1 = fgetoff(fd1);
  printf(1, "OFFSET  FD1 FD1 FD1 FD1 FD1 FD1 FD1 %d\n", off1);

  fseek(fd0,1);
  off0 = fgetoff(fd0);
  printf(1, "OFFSET  FD0 FD0 FD0 FD0 FD0 FD0 FD0 %d\n", off0);


  fseek(fd1,5);
  off1 = fgetoff(fd1);
  printf(1, "OFFSET  FD0 FD0 FD0 FD0 FD0 FD0 FD0 %d\n", off1);

  close(fd0);
  close(fd1);

  exit();
}
