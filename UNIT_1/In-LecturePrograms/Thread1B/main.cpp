#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NTHREADS 9

struct temp
{
	int value;
	char ch;
};

void * inc_x(void *x_void_ptr)
{
	struct temp *x_ptr = (struct temp *)x_void_ptr;
	printf("Char = %c, value = %d\n",x_ptr->ch,x_ptr->value);
	return NULL;
}



int main()
{
	static int x[NTHREADS] = {0};
	pthread_t tid[NTHREADS];
//	printf("Initial value of x: %d\n", x);
	static struct temp y[NTHREADS];
	for (int i = 0; i < NTHREADS; i++)
	{
		y[i].value=i;
		y[i].ch='0'+i;
		if (pthread_create(&tid[i], NULL, inc_x, &y[i]))
		{
			fprintf(stderr, "Error creating thread\n");
			return 1;
		}
		
	}
	for (int i = 0; i < NTHREADS; i++)
		pthread_join(tid[i], NULL);
//	for (int i = 0; i < NTHREADS; i++)
//		printf("Final value of x[%d]: %d\n",i, x[i]);
	return 0;
}