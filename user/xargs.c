#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {

    char buf[1024];
    char params[MAXARG][64];
    int paramCount = 0;
    int cur = 0;
    memset(buf, 0, sizeof(buf));
    int len = 0;
    while (read(0, buf + len, 1) > 0)
    {
        len++;
    }

    for (int i = 0; i < sizeof(buf); i++) {
        if (buf[i] == '\n') {
            params[paramCount][cur] = 0;
            ++paramCount;
            cur = 0;
        } else {
            params[paramCount][cur++] = buf[i];
        }
    }
    char * p[paramCount + argc];
    
    for (int i = 1; i < argc; i++) {
        p[i - 1] = argv[i];
    }

    for (int i = argc - 1; i < argc + paramCount - 1; i++) {
        p[i] = params[i - argc + 1];
    }
    p[argc + paramCount - 1] = 0;
    if (fork() > 0) {

        wait(0);
    } else {
        exec(argv[1], p);
    }

    exit(0); 
}