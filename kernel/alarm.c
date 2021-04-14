#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "stat.h"
#include "proc.h"

uint64 sys_sigalarm(void) {
    int itv;
    uint64 handler;
    struct proc* p = myproc();
    argint(0, &itv);
    argaddr(1, &handler);
    p->interval = itv;
    p->handler = handler;
    p->flag = 0;
    return 0;
}

uint64 sys_sigreturn(void) {
    struct proc* p = myproc();
    p->trapframe->epc = p->u_epc;
    p->trapframe->ra = p->u_ra;
    p->trapframe->sp = p->u_sp;
    p->trapframe->gp = p->u_gp;
    p->trapframe->tp = p->u_tp;
    p->trapframe->t0 = p->u_t0;
    p->trapframe->t1 = p->u_t1;
    p->trapframe->t2 = p->u_t2;
    p->trapframe->s0 = p->u_s0;
    p->trapframe->s1 = p->u_s1;
    p->trapframe->a0 = p->u_a0;
    p->trapframe->a1 = p->u_a1;
    p->trapframe->a2 = p->u_a2;
    p->trapframe->a3 = p->u_a3;
    p->trapframe->a4 = p->u_a4;
    p->trapframe->a5 = p->u_a5;
    p->trapframe->a6 = p->u_a6;
    p->trapframe->a7 = p->u_a7;
    p->trapframe->s2 = p->u_s2;
    p->trapframe->s3 = p->u_s3;
    p->trapframe->s4 = p->u_s4;
    p->trapframe->s5 = p->u_s5;
    p->trapframe->s6 = p->u_s6;
    p->trapframe->s7 = p->u_s7;
    p->trapframe->s8 = p->u_s8;
    p->trapframe->s9 = p->u_s9;
    p->trapframe->s10 = p->u_s10;
    p->trapframe->s11 = p->u_s11;
    p->trapframe->t3 = p->u_t3;
    p->trapframe->t4 = p->u_t4;
    p->trapframe->t5 = p->u_t5;
    p->trapframe->t6 = p->u_t6;
    p->flag = 0;
    return 0;
}