#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char **argv) {
    int fd = open("a", 0);
    char buf[512];
    if (read(fd, buf, sizeof(buf)) == 0) {
        printf("123");
    };
    fprintf(1, buf);
    return 0;
}