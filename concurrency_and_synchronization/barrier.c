/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "spinlock.h"
#include "defs.h"
#include "barrier.h"

//define any variables needed here
int count = 0;
int max_count;
struct spinlock lock;


int
barrier_init(int n)
{
  initlock(&lock, "my_lock");
  max_count = n;
  return 0;
}

int
barrier_check(void)
{
  acquire(&lock);
  count+=1;
  while(count<max_count){
    sleep(&max_count, &lock);
  }
  wakeup(&max_count);
  release(&lock);
  return 0;
}

/*----------xv6 sync lock end----------*/
