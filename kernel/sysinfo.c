#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"
#include "defs.h"
#include "sysinfo.h"

uint64 sys_sysinfo(void) {
    struct sysinfo i;
    uint64 user;
    if (argaddr(0, &user) < 0)
        return -1;
    //printf("in syscall the addr is :%d\n", user);
    i.freemem = getfreemem();
    i.nproc = getnproc();
    struct proc *p = myproc();
    //printf("freemen:%d\n", i.freemem);
    //printf("nproc:%d\n", i.nproc);
    if (copyout(p->pagetable, user, (char*)&i, sizeof(struct sysinfo)) < 0)
        return -1;
    return 0;
}
