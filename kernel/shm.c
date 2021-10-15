#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>

static unsigned long phys_addr = 0;


int sys_shmget() {
  if (!phys_addr) {
     phys_addr = (unsigned long)get_free_page();
  }
	unsigned long data_base = get_base(current->ldt[2]);
  unsigned long shm_addr = current->brk + PAGE_SIZE;
  printk("set shm address %lu, %lu!\n", shm_addr, phys_addr);
  current->brk += PAGE_SIZE;
	put_page(phys_addr, data_base + shm_addr);
  return (int)shm_addr;
}
