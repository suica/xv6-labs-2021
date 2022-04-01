#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if (argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;

  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// #ifdef LAB_PGTBL
int sys_pgaccess(void)
{
  uint64 start_addr;
  if (argaddr(0, &start_addr) < 0)
  {
    return -1;
  }
  int page_num;
  if (argint(1, &page_num) < 0)
  {
    return -1;
  }
  uint64 result_mask_addr;
  if (argaddr(2, &result_mask_addr) < 0)
  {
    return -1;
  }
  should_set_access_bit = 0;
  struct proc *p = myproc();
  vmprint(p->pagetable);
  uint64 result = pgaccess(p->pagetable, start_addr, page_num);
  if (copyout(p->pagetable, result_mask_addr, (char *)&result, sizeof(result)) < 0)
  {
    return -1;
  }
  should_set_access_bit = 1;
  return 0;
}
// #endif

uint64
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
