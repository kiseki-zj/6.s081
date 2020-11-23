#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
int is_match(char* match, char* str) {
    //printf("%s %s\n", match, str);
    int match_len = strlen(match);
    int str_len = strlen(str);
    char substr[20];
    if (match_len > str_len) return 0;
    for (int i = 0; i < str_len - match_len + 1; i++) {
        memcpy(substr, str + i, match_len);
        substr[match_len] = 0;
        if (strcmp(substr, match) == 0)
        return 1;
    }
    return 0;
}
void find(char* path, char* match) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find1: cannot stat %s\n", path);
        close(fd);
        return;
    }
    
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      //printf("%s:de.inum = %d\n", fmtname(buf), de.inum);
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      if(de.inum == 0)
        continue;
      //printf("=============de.name = %s\n", de.name);
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      //printf("=============buf = %s\n", buf);
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == 2) {
          if (is_match(match, de.name)) {
              fprintf(1, "%s\n", buf);
          }
      }
      else if (st.type == 1) {
          find(buf, match);
      }
    }
    close(fd);
}

int main(int argc, char** argv) {
    //printf("%d\n", argc);
    find(argv[1], argv[2]);
    exit(0);
}