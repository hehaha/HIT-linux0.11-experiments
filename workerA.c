#define __LIBRARY__
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


_syscall0(int, shmget)


int main()
{
    unsigned long i = 5;
    unsigned long *shm_p = NULL;
    printf("shm addr value: 0x%08x, 0x%08x\n", &i, &shm_p);
    shm_p = (unsigned long *)shmget();
    printf("shm addr value: 0x%08x\n", shm_p);
    printf("shm addr value: %d, %d, %lu\n", *shm_p != 2, *shm_p == 0, *shm_p);
    while (*shm_p != 2) {
      if (*shm_p == 0) {
        *shm_p = 1;
        printf("set shm value 1");
        fflush(stdout);
      }
    }
    printf("worker a finished");
    fflush(stdout);
    return 0;
}
