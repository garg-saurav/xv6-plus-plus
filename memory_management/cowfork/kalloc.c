// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  int numfree;
  int refcount[(int)(PHYSTOP/PGSIZE)]; // indexed by physical address of a page
} kmem;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.numfree = 0; // initialisation
  kmem.use_lock = 0;
  for(int i=0; i<(int)(PHYSTOP/PGSIZE); i++){
    kmem.refcount[i] = 0;
  }
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;
  int done = 0;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP) 
    panic("kfree");

  if(kmem.use_lock)
    acquire(&kmem.lock);

  if(kmem.refcount[RCX(V2P(v))]>1){
    kmem.refcount[RCX(V2P(v))]-=1; // reduce 1 reference
    done = 1;
  }

  // if(kmem.use_lock)
  //   release(&kmem.lock);

  if (done==1){
    if(kmem.use_lock)
      release(&kmem.lock);
    return;
  }


  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  // if(kmem.use_lock)
  //   acquire(&kmem.lock);
  
  kmem.refcount[RCX(V2P(v))]=0; // no more reference
  kmem.numfree+=1; // add 1 to free pages count
  r = (struct run*)PGROUNDDOWN((uint)v);
  r->next = kmem.freelist;
  kmem.freelist = r;
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r){
    kmem.freelist = r->next;
    kmem.numfree--; // decrease free pages count 
    kmem.refcount[RCX(V2P(r))] = 1; // set reference count to 1
  }
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

// return number of free pages
int 
getNumFree(void)
{
  
  return kmem.numfree; 

}

// increase ref count by 1
void
refinc(char* v)
{
  // assume lock is held before calling this

  kmem.refcount[RCX(V2P(v))]+=1;

}

// decrease ref count by 1
void
refdec(char* v)
{
  // assume lock is held before calling this

  kmem.refcount[RCX(V2P(v))]-=1;
}

// get reference count
int
getrefcount(char* v)
{
  
  return kmem.refcount[RCX(V2P(v))];
}

// get kernel lock
void
getkernellock()
{
  if(kmem.use_lock)
    acquire(&kmem.lock);
}

// release kernel lock
void
releasekernellock()
{
  if(kmem.use_lock)
    release(&kmem.lock);
}