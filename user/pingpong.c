#include "kernel/types.h"
#include "user.h"

int main(int argc, char** argv) {
    char mes = 10;
    int p[2];
    char rec;
    pipe(p);
    if (fork() != 0) {
        write(p[1], &mes, 1);
        wait(0);
        if (read(p[0], &rec, 1) != 0) {
            printf("%d: received pong\n", getpid());
            exit(0);
        }
    }
    else {
        if (read(p[0], &rec, 1) != 0) {
            printf("%d: received ping\n", getpid()); 
            write(p[1], &rec, 1);
            exit(0);          
        }
    }
    return 0;
}