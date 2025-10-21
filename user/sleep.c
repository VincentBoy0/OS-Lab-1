#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "command type is wrong...\n");
        exit(1);
    }
    int counter = atoi(argv[1]);
    sleep(counter * 10);
    exit(0);
}