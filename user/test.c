#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int i = 0;
void f() {
    i++;
    f();
}
int main() {
    f();
    exit(0);
}