#include <iostream>
#include <unistd.h>

int main()
{
    pid_t pid;
    for (int i = 0; i < 3; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            if (i == 0 || i == 2)
            {
                pid = fork();
                if (pid == 0)
                {
                    std::cout << "I grandchild process 1 from child process " << i + 1 << std::endl;
                    
                    _exit(0);
                }
                wait(0);
            }
            else
            {
                for (int j = 0; j < 2; j++)
                {
                    pid = fork();
                    if (pid == 0)
                    {
                        std::cout << "I grandchild process " << j + 1 << "from child process " << i + 1 << std::endl;
                        _exit(0);
                    }
                }
                for (int j = 0; j < 2; j++)
                    wait(0);
            }
            std::cout << "I am child process " << i + 1 << std::endl;
            _exit(0);
        }

    }
    for (int i = 0; i < 3; i++)
        wait(0);
    std::cout << "I am the parent process " << std::endl;
    return 0;