#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

void fireman(int) {
    // waitpid is a version of wait that waits for a particular pid value (first param)
    // -1 waits for ANY child process
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        std::cout << "A child process ended" << std::endl;
    }
}

int main() {
    signal(SIGCHLD, fireman);
    int nChild = 0;

    while (true) {
        if (fork() == 0) {
            std::cout << "A child process started" << std::endl;
            sleep(1);
            _exit(0);
        }
        std::cin.get();
        // nChild ++;
        // if (nChild == 10) {
        //     break;
        // }
    }
    
    std::cout << "Parent process ended" << std::endl;
}