#define __LIBRARY__
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


_syscall0(int, shmget)


int main()
{
  unsigned long *shm_p = NULL;
  shm_p = (unsigned long *)shmget();
  printf("shm addr value: %lu\n", *shm_p);
  fflush(stdout);
  while (*shm_p != 1) {};
  *shm_p = 2;
  printf("set shm value 2. worker b finished");
  fflush(stdout);
  while(1) {};
  return 0;
}
