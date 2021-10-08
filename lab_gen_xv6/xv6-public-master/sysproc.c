#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"


#include "vga.h"


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

// MIs syscall


int 
sys_modeswitch(void)
{
  int modo;
  
  argint(0, &modo);
  
  if (modo == 1){
	  write_regs(g_320x200x256);
      return 0;
  }
  else if (modo == 0){
	  write_regs(g_80x25_text);
	  write_font(g_8x16_font, 16);
      return 0;
  }
  else{
    return 0;
  }

}

int 
sys_plotpixel(void)
{
    int x;
    int y;
    int colour;

    argint(0, &x);
    argint(1, &y);
    argint(2, &colour);

    uchar* location = P2V((uchar*)0xA0000 + 320 * y + x);
    *location = colour;
    return 0;
}



