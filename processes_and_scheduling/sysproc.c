#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "processInfo.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// print Hello to the console
int
sys_hello(void)
{
	cprintf("Hello\n");
	return 0;
}

// print string name to the console
int
sys_helloYou(void)
{
	char *name;

	if(argstr(0, &name) < 0)
    	return -1;

    cprintf("%s\n",name);

	return 0;
}

// return number of active processes 
// either in embryo, running, runnable, sleeping, or zombie states
int
sys_getNumProc(void)
{
	return getNumProc();
}

// return maximum pid amongst all active processes 
// either in embryo, running, runnable, sleeping, or zombie states
int
sys_getMaxPid(void)
{
	return getMaxPid();
}

// get the process information in the processInfo struct
int
sys_getProcInfo(void)
{
	struct processInfo* procInfo;
	int pid;

	if(argint(0, &pid) < 0 || argptr(1, (void*)&procInfo, sizeof(*procInfo)) < 0)
    	return -1;

   	return getProcInfo(pid, procInfo);

}

// set priority
int
sys_setprio(void)
{
	int prio;

	if(argint(0, &prio) < 0)
    	return -1;

    return setprio(prio);

}

// get priority
int
sys_getprio(void)
{
	return myproc()->priority;
}