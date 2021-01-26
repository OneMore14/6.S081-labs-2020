#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        
        printf("no argumnert, usage: sleep 10\n");
    }else {

        int sleep_time = atoi(argv[1]);
        sleep(sleep_time);
    }
    exit(0);
}