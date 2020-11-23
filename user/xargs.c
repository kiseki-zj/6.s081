#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char** argv) {
    char* args[MAXARG];
    args[0] = argv[1];
    char buf[32];
    int i;
    for (i = 2; i < argc; i++) {
        args[i - 1] = argv[i];
    }
    
    while (gets(buf, sizeof(buf))) {
        //fprintf(2, "buf = %s\n", buf);
        if (strlen(buf) == 0) break;
        int j  = 0;
        while (buf[j] && buf[j] != '\n') j++;
        buf[j] = 0;
        i = argc - 1; 
        //args[i] = (char*)malloc(sizeof(char) * 32); 
        //strcpy(args[i], buf);
        args[i++] = buf;
        args[i] = 0;        
        if (fork() == 0) {
            exec(args[0], args);
            exit(0);
        }
        else {
            wait(0);
        }
    }
    exit(0);
}
