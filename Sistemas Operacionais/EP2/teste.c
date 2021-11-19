#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
 
void changeValues (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void ArrayDisplay (int arr1[], int n)
{
	printf("The shuffled elements in the array are:  \n");    
    for (int i = 0; i < n; i++)
        printf("%d ", arr1[i]);
    printf("\n");
}
void shuffleRandon ( int arr1[], int n )
{
    srand ( time(NULL) );
    for (int i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        changeValues(&arr1[i], &arr1[j]);
    }
}
int main()
{
    /* int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int n = sizeof(arr1)/ sizeof(arr1[0]);
	int i;
 //------------- print original array ------------------	
	printf("The given array is:  \n");
	for(i = 0; i < n; i++)
		{
			printf("%d  ", arr1[i]);
		}
	printf("\n");
//------------------------------------------------------ 		
    shuffleRandon (arr1, n);
    ArrayDisplay(arr1, n);
    return 0; */
    pthread_mutex_t sem;

    pthread_mutex_init (&sem, NULL);

    if (sem.__data.__lock == 1)
        printf ("O semáforo tá lockado\n");
    else 
        printf ("O semáforo tá unlockado\n");
    
    pthread_mutex_lock (&sem);
    printf ("Lockei o semáforo\n");

    if (sem.__data.__lock == 1)
        printf ("O semáforo tá lockado\n");
    else 
        printf ("O semáforo tá unlockado\n");

    pthread_mutex_unlock (&sem);

    printf ("O semáforo unlockado vale: %d\n", sem.__data.__lock);

    // pthread_mutex_lock (&sem);

    // printf ("O semáforo lockado vale: %d\n", sem.__data.__lock);

    if (pthread_mutex_trylock (&sem) != 0)
        printf ("Deu merda, o bangui ja ta lockado !!\n");
    else
        printf ("Lockei mermo fodasse!\n");

    if (sem.__data.__lock == 1)
        printf ("O semáforo tá lockado\n");
    else 
        printf ("O semáforo tá unlockado\n");

    // printf ("mutex_lock == %d\n", sem.__data.__lock);

    pthread_mutex_destroy (&sem);
}
