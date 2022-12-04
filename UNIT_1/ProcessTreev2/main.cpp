// I am the parent process 
// I am the child process 0
// I am the grandchild process from child process 0
// I am the child process 1
// I am the child process 2
// I am the grandchild process from child process 2
// I am the grandchild process from child process 2

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    std::cout << "I am the parent process " << std::endl;
    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0) {
            std::cout << "I am the child process " << i << std::endl;
            if (i == 0) {
                pid = fork();
                if (pid == 0) {
                    std::cout << "I am the grandchild process from child process " << i << std::endl;
                    _exit(0);
                }
                wait(nullptr);
            }
            else if (i == 2) {
                for (int j = 0; j < 2; j++) {
                    pid = fork();
                    if (pid == 0) {
                        std::cout << "I am the grandchild process from child process " << i << std::endl;
                        _exit(0);
                    }
                }
                wait(nullptr);
            }
            _exit(0);
        }
        wait(nullptr);
    }
    return 0;
}