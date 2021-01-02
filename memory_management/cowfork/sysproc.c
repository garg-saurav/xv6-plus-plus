#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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

// get number of free pages
int
sys_getNumFreePages(void)
{
  return getNumFree();
}

// get number of free virtual pages in uvm
int
sys_getNumVirtPages(void)
{
  return PGROUNDUP(myproc()->sz)/PGSIZE;
}

// get number of physical pages in uvm
int
sys_getNumPhysPages(void)
{
  int count=0;
  pte_t *pgtab;
  pde_t *pgdir;

  pgdir = myproc()->pgdir;
  
  if(pgdir==0){
    panic("sys_getNumPhysPages: no pgdir");
  } else {
    for(int j=0; j<NPDENTRIES; j++){
      if(pgdir[j] & PTE_P && pgdir[j] & PTE_U){
        pgtab = (pte_t*)P2V(PTE_ADDR(pgdir[j]));
        for(int k=0; k<NPTENTRIES; k++){
          if(pgtab[k] & PTE_P && pgtab[j] & PTE_U){
            count+=1;
          }
        }
      }
    }
  }
  return count;
}

// get number of pages consumed by page table of a process 
int
sys_getNumPTPages()
{
  int count=0;
  pde_t *pgdir;

  pgdir = myproc()->pgdir;

  if(pgdir==0){
    return count;
  }else{
    count+=1;
    for(int j=0; j<NPDENTRIES; j++){
      if(pgdir[j] & PTE_P){
        count+=1;
      }
    }
    return count;
  }
}