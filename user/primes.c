#include "kernel/types.h"
#include "user.h"

#define R 0
#define W 1
int main() {
    int num[40];
    int first_prime;
    char is_prime[40];
    int p[2];
    int nums;

    pipe(p);
    num[0] = 34;
    for (int i = 2; i <= 35; i++) {
        num[i - 1] = i;
    }
    for (int i = 0; i <= num[0]; i++) 
        write(p[W], &num[i], sizeof(int));
    while (num[0]) {
        int pid = fork();
        if (pid == 0) {
            read(p[R], num, sizeof(int));
            read(p[R], num + 1, sizeof(int) * num[0]);
            memset(is_prime, 0, sizeof(is_prime));
            nums = 0;
            first_prime = num[1];
            printf("prime %d\n", first_prime);
            for (int i = 1; i <= num[0]; i++)
                if (num[i] % first_prime) {
                    is_prime[i] = 1;
                    nums++;
                }
            write(p[W], &nums, sizeof(int));
            for (int i = 1; i <= num[0]; i++) 
                if (is_prime[i])
                    write(p[W], &num[i], sizeof(int));
            num[0] = nums;
        }
        else {
            wait(0);
            exit(0);
        }
    }
    exit(0);
    /*first_num = num[1];
    for (int i = 1; i <= num[0]; i++) 
        if (num[i] % first_num) {
            write(p[W], num[i], sizeof(int));
        }*/
    return 0;
}