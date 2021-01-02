/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "uspinlock.h"

//TODO: define an array of NLOCK uspinlocks
struct userspinlock {
	uint locked; // Is the lock held?
};
struct userspinlock uspinlocks[NLOCK];

int
uspinlock_init(void)
{
  // Initialize all locks to unlocked state
  for(int i=0; i<NLOCK; i++)
    uspinlocks[i].locked = 0;
  return 0;
}

int
uspinlock_acquire(int index)
{
  while(xchg(&uspinlocks[index].locked, 1)!=0)
    ; // spin
  return 0;
}

int
uspinlock_release(int index)
{
  xchg(&uspinlocks[index].locked, 0);
  return 0;
}

int
uspinlock_holding(int index)
{
  //Return 0 if lock is free, 1 if lock is held
  return uspinlocks[index].locked==1;
}
/*----------xv6 sync lock end----------*/
