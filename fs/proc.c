#ifndef __LIBRARY__
#define __LIBRARY__
#endif

#include <unistd.h>
#include <linux/sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <asm/segment.h>


_syscall2(int,mkdir,const char*,name,mode_t,mode)
_syscall3(int,mknod,const char*,filename,mode_t,mode,dev_t,dev)

static const char *PROC_HEADER = "pid state father counter start_time\n";
static const long HEADER_LEN = 36;
long update_time = 0;
char proc_info[1000] = {'\0',};
long info_len = 0;


void init_proc(void) {
    mkdir("/proc", 0755);
    mknod("/proc/psinfo", S_IFPROC | 0444, 0);
}


int num2char(long num, char *buf) {
    if (!num) {
        *buf = '0';
        return 1;
    }
    char c = '0';
    char* start = buf;
    char* end = buf;
    int count = 0;
    if (num < 0) {
        *start = '-';
        num = -num;
        count++;
        start++;
        end++;
    }
    while (num) {
        int val = num % 10;
        *end = c + val;
        end++;
        num /= 10;
        count++;
    }
    while (start < end) {
        end--;
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
    }
    return count;
}


int proc_read(char *buf, int nr, long *pos) {
    if (*pos >= info_len + HEADER_LEN) {
        return 0;
    }
    if (jiffies - update_time > 100) {
        info_len = 0;
    }
    if (!info_len) {
        int offset = 0;
        int i = 0;
        for(i = 0; i < NR_TASKS; i++) {
            struct task_struct *t = task[i];
            if (!t)
                continue;
            offset += num2char(t->pid, proc_info + offset);
            proc_info[offset++] = ' ';
            offset += num2char(t->state, proc_info + offset);
            proc_info[offset++] = ' ';
            offset += num2char(t->father, proc_info + offset);
            proc_info[offset++] = ' ';
            offset += num2char(t->counter, proc_info + offset);
            proc_info[offset++] = ' ';
            offset += num2char(t->start_time, proc_info + offset);
            proc_info[offset++] = '\n';
        }
        info_len = offset;
        proc_info[offset] = '\0';
    }
    int left = nr;
    if (*pos < HEADER_LEN)  {
        int idx = *pos;
        while (left-- > 0 && idx < HEADER_LEN) {
            char c = PROC_HEADER[idx++];
            put_fs_byte(c, buf++);
        }
        *pos += idx;
    }
    int idx = *pos - HEADER_LEN;
    while (left-- > 0 && idx < info_len) {
        char c = proc_info[idx++];
        put_fs_byte(c, buf++);
    }
    *pos += idx;
    return nr > HEADER_LEN ? HEADER_LEN + idx : nr;
}
