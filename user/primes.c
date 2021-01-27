#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


#define STDDER_FILENO 2

#define READEND 0
#define WRITEEND 1

void f(int lfd[]) {

       close(lfd[1]);
        int prime = 0;
        int len = read(lfd[0], &prime, sizeof(int));
        if (len == 0) {
            exit(0);
        }
        printf("prime %d\n", prime);

        int rfd[2];
        pipe(rfd);
        int num;

        while (read(lfd[0], &num, sizeof(int)))
        {
            if (num % prime > 0) {
                write(rfd[1], &num, sizeof(int));
            }
        }
        close(lfd[0]);
        close(rfd[1]);
        if (fork() > 0) {
            wait(0);
            exit(0);
        } else {
            f(rfd);
        }

}

int main(void)
{
    int fd[2];
    pipe(fd);
    for (int i = 2; i <= 35; i++) {
        write(fd[1], &i, sizeof(i));
    }

    if (fork() > 0) {

        close(fd[0]);
        close(fd[1]);
        wait(0);
        exit(0);        
    } else {
        f(fd);
        exit(0);
    }   
}
