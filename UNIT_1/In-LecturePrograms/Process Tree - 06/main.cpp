#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
	pid_t pid;
	std::cout<< "I am the parent process"<< std::endl;
	for (int i=0;i<3;i++)
	{
		pid = fork();
		if (pid == 0)
		{
			std::cout << "I am child process " << i << std::endl;
			if (i%2 == 0)
			{
				for (int j=0;j<2;j++)
				{
					pid = fork();
					if (pid == 0)
					{
						std::cout << "I am grandchild process " << j << " from child process " << i << std::endl;
					//	sleep(5);
						_exit(0);
					}
					wait(NULL);
				}
			//	for(int j=0;j<2;j++)
			//		wait(NULL);
			}
			else
			{
				pid = fork();
				if (pid == 0)
				{
						std::cout << "I am grandchild process 0 from child process " << i << std::endl;
				//		sleep(5);
						_exit(0);
				}
				wait(NULL);
			}
			
		//	sleep(5);
			_exit(0);
		}
		wait(nullptr);
	}
//	for (int i=0;i<3;i++)
//		wait(NULL);

//	sleep(5);
	return 0;
}