#include "kernel/types.h"
#include "user.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(2, "sleep: error\n");
        exit(1);
    }
    else {
        uint times = atoi(argv[1]);
        sleep(times);
        exit(0);
    }
}