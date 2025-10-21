#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void backtrack(int read_fd) {
    int fPrime;
    int n = read(read_fd, &fPrime, sizeof fPrime);
    if (n == 0) {
        close(read_fd);
        exit(0);
        return;
    }
    printf("prime %d\n", fPrime);

    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        close(read_fd);
        backtrack(p[0]);
        exit(0);
    }
    else {
        close(p[0]);
        int prime;
        while((read(read_fd, &prime, sizeof prime)) > 0) {
            if (prime % fPrime == 0) continue;
            write(p[1], &prime, sizeof prime);
        }
        close(p[1]);
        close(read_fd);
        wait(0);
    }
}
int main(int argc, char* argv[]) {
    if (argc > 1) {
        fprintf(2, "command type is wrong...\n");
        exit(1);
    }
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        backtrack(p[0]);
        exit(0);
    }
    else {
        close(p[0]);
        for(int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof i);
        }
        close(p[1]);
        wait(0);
    }
}
