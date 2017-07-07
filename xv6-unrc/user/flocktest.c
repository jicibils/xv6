#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "syscall.h"
int
main(void)
{
  int pid, fd, r, w;

  printf(1, "flock test\n");

  fd = open("arch", O_CREATE);
  r = open("arch", O_CREATE);
  w = open("arch", O_CREATE);
  printf(1, "FD FD FD FD FD FD FD %d\n", fd);
  printf(1, "r r r r r r r %d\n", r);
  printf(1, "w w w w w w w %d\n", w);

  close(r);
  close(w);
  close(fd);
  semfree(fd);

  printf(1, "FD FD FD FD FD FD FD %d\n", fd);
  printf(1, "r r r r r r r %d\n", r);
  printf(1, "w w w w w w w %d\n", w);

  w = open("arch", O_CREATE);
  r = open("arch", O_CREATE);
  fd = open("arch", O_CREATE);

  printf(1, "w w w w w w w %d\n", w);
  printf(1, "r r r r r r r %d\n", r);
  printf(1, "FD FD FD FD FD FD FD %d\n", fd);



 
  pid = fork();
  if ( pid == 0){
    printf(1, "Pid CHILD %d\n", pid);
	  w = open("arch", O_RDWR);

	  if(flock(w) == -1){
		  printf(1, "ERROR FLOCK\n");
			exit();
		}

	  int b = 7;
	  write(w, &b, 1);  
	  printf(1,"-------------------------- DESPUES DEL READ: %d\n",b);
		funlock(w);

	}else{
    printf(1, "Pid PARENT %d\n", pid);
	  w = open("arch", O_RDWR);

	  if(flock(w) == -1){
		  printf(1, "ERROR FLOCK\n");
			exit();
		}

	  int b = 7;
	  write(w, &b, 1);  
	  printf(1,"-------------------------- DESPUES DEL READ: %d\n",b);
		funlock(w);

	}

  close(r);
  close(w);
  close(fd);
  exit();
}
