// The multithreaded calculator.
// You must complete the program below (using POSIX threads) that creates four threads to calculate the addition, subtraction, multiplication, and division of a set of numbers read from STDIN.

// The input of the program will be four pair of integer values with the following format:
// 1 2
// 2 3
// 4 5
// 6 7
// Where the first line has the values to execute the addition, the second line has the values to execute the subtraction, the third line has the values to execute the multiplication, and the last line has the values to execute the division. For the division operator, if the denominator is equal to zero, the result of the operation is zero. 
// Given the previous input, the expected output is:
// 1 + 2 = 3.00
// 2 - 3 = -1.00
// 4 * 5 = 20.00
// 6 / 7 = 0.86
// Notes: 

// Use the comments in the provided template file to complete your solution.
// Not using POSIX threads will translate into a penalty of 100%.
// You can always assume that the input will be valid.

#include <pthread.h>
#include <iostream>
#include <iomanip>

#define NOPER 4

struct operation
{
	int val1;
	int val2;
    int op;
    double result;
};

void *calculator(void *pos_void_ptr)
{
	struct operation *pos_ptr = (struct operation *)pos_void_ptr; // cast pos_void_ptr to a struct operation *
    switch(pos_ptr->op)
    {
        case 0: pos_ptr->result = pos_ptr->val1 + pos_ptr->val2; 
                break;
        case 1: pos_ptr->result = pos_ptr->val1 - pos_ptr->val2; 
                break;
        case 2: pos_ptr->result = pos_ptr->val1 * pos_ptr->val2; 
                break;
        case 3: if (pos_ptr->val2 != 0)
                    pos_ptr->result = (double) pos_ptr->val1 / pos_ptr->val2; 
                else
                    pos_ptr->result = 0;         
                break;
    }
	return NULL;
}

int main()
{
	static struct operation operations[NOPER];
 	pthread_t tid[NOPER];
        
	for(int i=0;i<NOPER;i++)
	{
		operations[i].op = i;
        std::cin >> operations[i].val1;
        std::cin >> operations[i].val2;
		if(/* Call pthread_create */pthread_create(&tid[i], nullptr, calculator, &operations[i])) 
		{
			fprintf(stderr, "Error creating thread\n");
			return 1;

		}		
	}
	
	// Wait for the other threads to finish.
    // Call pthread_join here
    for (int i = 0; i < NOPER; i++) 
        pthread_join(tid[i], NULL);
    for (int i = 0; i < NOPER; i++)
    {
        switch(operations[i].op)
        {
            case 0: std::cout << operations[i].val1 << " + " << operations[i].val2 << " = " << std::fixed << std::setprecision(2) << operations[i].result << std::endl;
                    break;
            case 1: std::cout << operations[i].val1 << " - " << operations[i].val2 << " = " << std::fixed << std::setprecision(2) << operations[i].result << std::endl;
                    break;
            case 2: std::cout << operations[i].val1 << " * " << operations[i].val2 << " = " << std::fixed << std::setprecision(2) << operations[i].result << std::endl;
                    break;
            case 3: std::cout << operations[i].val1 << " / " << operations[i].val2 << " = " << std::fixed << std::setprecision(2) << operations[i].result << std::endl;
                    break;
        }
    }
    return 0;
}