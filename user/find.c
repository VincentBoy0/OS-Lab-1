#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* getname(char *path) {
    static char buf[DIRSIZ+1];
    char *p;

    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;

    if(strlen(p) >= DIRSIZ) return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    for(int i = 0; i < strlen(buf); i++) if (buf[i] == ' ') {
        buf[i] = '\0';
        break;
    }
    return buf;
}

void dfs(char *path, char* file_name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "cannot stat %s\n", path);
        close(fd);
        return;
    }
    
    if (st.type == T_DEVICE || st.type == T_FILE) {
        if (strcmp(getname(path), file_name) == 0) {
            printf("%s\n", path);
        }
    }
    else if (st.type == T_DIR) {
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("path too long\n");
        }
        else {
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0) continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("cannot stat %s\n", buf);
                    continue;
                }
                if (getname(buf)[0] == '.') continue;
                dfs(buf, file_name);
            }
        }
    }
    close(fd);
}
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("command type is wrong \n");
        return 0;
    }
    dfs(argv[1], argv[2]);
}