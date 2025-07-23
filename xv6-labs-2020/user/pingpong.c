#include "kernel/types.h"
#include "user/user.h"

#define Buffer_size 256

int my_strlen(const char *s) {
    int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

int main(int argc, char * argv[]) {
    if (argc != 1) {
        printf("too many arguments, Right Usage: %s\n", argv[0]);
        exit(1);
    }

    int ptoc[2];
    int ctop[2];
    if ((pipe(ptoc) == -1) || (pipe(ctop) == -1)) {
        printf("pipe init error\n");
        exit(1);
    }

    int pid = fork();
    if (pid == -1) {
        printf("fork process error\n");
        exit(1);
    }
    // 子进程
    else if (pid == 0) {
        const char* msg_to_parent = "pong";    
        char buffer[Buffer_size];
        close(ptoc[1]);
        close(ctop[0]);

        int bytes_read = read(ptoc[0], buffer, Buffer_size - 1);
        if (bytes_read > 0 && bytes_read < Buffer_size)
            buffer[bytes_read] = '\0';
        else
            buffer[0] = '\0';

        printf("received message <%s> from parent process\n", buffer);
        close(ptoc[0]);

        write(ctop[1], msg_to_parent, my_strlen(msg_to_parent) + 1);
        close(ctop[1]);

        exit(0);
    }
    // 父进程
    else {
        const char* msg_to_child = "ping";
        char buffer[Buffer_size];

        close(ptoc[0]);
        close(ctop[1]);

        write(ptoc[1], msg_to_child, my_strlen(msg_to_child) + 1);
        close(ptoc[1]);

        int bytes_read = read(ctop[0], buffer, Buffer_size - 1);
        if (bytes_read > 0 && bytes_read < Buffer_size)
            buffer[bytes_read] = '\0';
        else
            buffer[0] = '\0';

        printf("received message <%s> from child process:%d\n", buffer, wait(0));
        close(ctop[0]);

        exit(0);
    }
}
