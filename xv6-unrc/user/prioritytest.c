#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
	int pid;
    pid = fork();
    if ( pid == 0){
	    printf(1, "Pid CHILD %d\n", pid);
	    setpriority(3);
	    procstat();
	}else{
	    printf(1, "Pid PARENT %d\n", pid);
	    setpriority(2);
	    procstat();

	}
    exit();

}
