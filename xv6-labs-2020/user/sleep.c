#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char * argv[]) {
    // 确保入参数量正确
    if (argc != 2) {
        printf("argument Number Error\n Right Usage: %s <ticks>\n" , argv[0]);
        exit(-1);
    }

    char *arg = argv[1];
    int ticks = atoi(arg);
    printf("Ready to sleep for %d ticks...\n", ticks);
    sleep(ticks);
    printf("Sleep finished\n");
    exit(0);
}