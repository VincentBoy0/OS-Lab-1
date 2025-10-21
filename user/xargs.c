#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/param.h"

char* strdup(const char* s) {
    int len = strlen(s);
    char* res = malloc(len + 1);
    for(int i = 0; i < len; i++) res[i] = s[i];
    res[len] = '\0';
    return res;
}
int main(int argc, char* argv[]) {
    char* args[MAXARG];
    for(int i = 1; i < argc; i++) args[i - 1] = argv[i];
    
    char c;
    int n = 0;
    char buffer[512];
    int newArgc = argc - 1;
    while(read(0, &c, sizeof c) > 0) {
        if (c == ' ' || c == '\n') {
            if (n == 0) continue;
            buffer[n] = '\0';
            args[newArgc++] = strdup(buffer);
            n = 0;
            if (c == '\n') {
                args[newArgc] = 0;
                if (fork() == 0) {
                    exec(argv[1], args);
                    exit(0);
                }
                wait(0);
                newArgc = argc - 1;
                n = 0;
            }
        }
        else {
            buffer[n++] = c;
        }
    } 
}