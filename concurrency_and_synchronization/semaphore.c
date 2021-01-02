/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "defs.h"
#include "semaphore.h"

int
sem_init(int index, int val)
{
  initlock(&semaphores[index].lock, "mylock");
  semaphores[index].value = val;
  return 0;
}

int
sem_up(int index)
{
  acquire(&semaphores[index].lock);
  semaphores[index].value+=1;
  wakeup_single(&semaphores[index].value);
  release(&semaphores[index].lock);
  return 0;
}

int
sem_down(int index)
{
  acquire(&semaphores[index].lock);
  while(semaphores[index].value<=0){
    sleep(&semaphores[index].value, &semaphores[index].lock);
  }
  semaphores[index].value-=1;
  release(&semaphores[index].lock);
  return 0;
}

/*----------xv6 sync lab end----------*/
