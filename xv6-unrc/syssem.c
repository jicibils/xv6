#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "semaphore.h"



int
sys_semget(void)
{
  int semid,initvalue;
  if(argint(0, &semid) < 0)
    return -1;
  if(argint(1, &initvalue) < 0)
    return -1;

  return semget(semid,initvalue);
}
int 
sys_semfree(void)
{
  int semid;
  if(argint(0, &semid) < 0)
    return -1;

  return semfree(semid);
}
int 
sys_semdown(void)
{
  int semid;
  if(argint(0, &semid) < 0)
    return -1;

  return semdown(semid);
}
int 
sys_semup(void)
{
  int semid;
  if(argint(0, &semid) < 0)
    return -1;

  return semup(semid);
}