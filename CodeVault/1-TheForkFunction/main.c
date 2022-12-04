#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // required library to use fork()

int main(int argc, const char * argv[]) {
    // clones the calling process, returns -1 for errors
    // 0 to the new process (child process)
    // the process ID of the new process to the old process (parent process)
    int id = fork();
    // printf("Hello world from id: %d\n", id);
    if (id == 0) {
        printf("Hello from child process\n");
    }
    else {
        printf("Hello from the main process\n");
    }
    
    // number of processes = 2^n fork() calls
//    fork();
//    fork();
//    printf("Hello world\n");
    
//    // call fork() only 3 times
//    int id = fork();
//    if (id != 0) {
//        fork();
//    }
//    printf("Hello world\n");
    return 0;
}
