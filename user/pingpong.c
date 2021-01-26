#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {

    int ping[2];
    int pong[2];
    pipe(ping);
    pipe(pong);
    char pingBuffer[5];
    char pongBuffer[5];
    if (fork() == 0) {

        read(ping[0], pingBuffer, 5);
        close(ping[0]);
        int sonId = getpid();
        printf("%d: received %s\n", sonId, pingBuffer);

        write(pong[1], "pong", 5);
        close(pong[0]);
    } else {

        write(ping[1], "ping", 5);
        close(ping[1]);

        read(pong[0], pongBuffer, 5);
        close(pong[0]);
        int fId = getpid();
        printf("%d: received %s\n", fId, pongBuffer);
    }
    exit(0);
}