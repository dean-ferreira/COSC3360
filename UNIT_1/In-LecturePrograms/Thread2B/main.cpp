#include <pthread.h>
#include <iostream>
#include <string>

#define NTHREADS 5
#define COUNT 5000


struct arg
{
    int value;
    std::string *array;
};

void *inc_x(void *x_void_ptr)
{

	struct arg *x_ptr = (struct arg *)x_void_ptr;
    *(x_ptr->array) = x_ptr->value+'0';
	return NULL;
}

int main()
{
	static struct arg x[5];
 	pthread_t tid[NTHREADS];
    std::string result[5];


	for(int i=0;i<NTHREADS;i++)
	{
        x[i].value = i;
        x[i].array = &result[i];
		if(pthread_create(&tid[i], NULL, inc_x, &x[i])) 
		{
			fprintf(stderr, "Error creating thread\n");
			return 1;

		}		

	}
	
	// Wait for the other threads to finish.
	for (int i = 0; i < NTHREADS; i++)
        	pthread_join(tid[i], NULL);
	for (int i = 0; i < NTHREADS; i++)
        std::cout << result[i] << std::endl;
	return 0;
}