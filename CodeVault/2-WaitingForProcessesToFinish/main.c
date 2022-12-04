#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, const char * argv[]) {
    // Print numbers 1 - 10, where 1 - 5 is printed by child process and 6 - 10 by main process
    int id = fork();
    int n;
    
    if (id == 0) {
        n = 1;
    }
    else {
        n = 6;
    }
    
    // stop executing until child process has finished executing
    if (id != 0) {
        wait(NULL);
    }
    
    for (int i = n; i < n + 5; i++) {
        printf("%d ", i);
        fflush(stdout);
    }
    
    if (id != 0) {
        printf("\n");
    }
    
    return 0;
}
