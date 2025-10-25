#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"


char buffer[512];
int main(int argc, char* argv[]) {
    if (argc > 1) {
        fprintf(2, "Usage: pingpong...\n");
        exit(1);
    }
    int n;
    // p1: parent -> write, child -> read
    // p2: parent -> read, child -> write
    int p1[2], p2[2]; 
    pipe(p1);
    pipe(p2);
    int f = fork();
    if (f < 0) {
        fprintf(2, "Fork failed!");
        exit(1);
    }
    else if (f == 0) {
        printf("%d: received ", getpid());
        close(p1[1]);
        while((n = read(p1[0], buffer, sizeof buffer)) > 0) {
            write(2, buffer, n);
        }
        close(p1[0]);
        // write(2, buffer, n);
        close(p2[0]);
        write(p2[1], "pong\n", 5);
        close(p2[1]);
        exit(0);
    }
    else {
        close(p1[0]);
        write(p1[1], "ping\n", 5);
        close(p1[1]);
        wait(0);
        close(p2[1]);
        printf("%d: received ", getpid());
        while((n = read(p2[0], buffer, sizeof buffer)) > 0) {
            write(2, buffer, n);
        }
        close(p2[0]);
    }
}