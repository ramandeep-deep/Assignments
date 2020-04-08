#include <pthread.h>		//Creating  POSIX threads.
#include <time.h>			//Waiting for  random  amount of time.
#include <unistd.h>			//Thread calls sleep for specific number of seconds.
#include <semaphore.h>		//creating semaphores
#include <stdlib.h>			
#include <stdio.h>			// To use Input Output


pthread_t TeachingAsst;		//Different  Thread for TeachingAsst.
pthread_t *Students;		// N  number of threads running as students.


int WorkingIndex = 0;
int NoOfChairs = 0;

//Declaraing of semaphores ,mutex lock.
sem_t TeachingAsst_Sleep;
sem_t Student_Sem;
sem_t ChairsSem[3];
pthread_mutex_t ChairObtained;
//Declaring  functions.
void *Teachingasst_Activity();
void *Student_Activity(void *threadID);

int main(int argc, char* argv[])
{
	int NumberOfStudents;		//a variable is taken from the user for creating student threads.
	int D_id;
	srand(time(NULL));

	//initialization( of mutex lock and  semaphores)
	sem_init(&TeachingAsst_Sleep, 0, 0);
	sem_init(&Student_Sem, 0, 0);
	for(D_id = 0; D_id < 3; ++D_id)			// Array of three Semaphores.
		sem_init(&ChairsSem[D_id], 0, 0);

	pthread_mutex_init(&ChairObtained, NULL);
	
	if(argc<2)
	{
		printf("No. of students");
        return 0;	
        }
	else
	{
		printf("No. of students considered. Creating %d threads.\n", NumberOfStudents);
		NumberOfStudents = atoi(argv[1]);
	}
	//allocating the memory(for students)
	Students = (pthread_t*) malloc(sizeof(pthread_t)*NumberOfStudents);

	//creating TeachingAssistent thread and N student threads.
	pthread_create(&TeachingAsst, NULL, Teachingasst_Activity, NULL);	
	for(D_id = 0; D_id < NumberOfStudents; D_id++)
		pthread_create(&Students[D_id], NULL, Student_Activity,(void*) (long)D_id);

	//Waiting ( for TeachingAssistent thread and N student threads.)
	pthread_join(TeachingAsst, NULL);
	for(D_id = 0; D_id < NumberOfStudents; D_id++)
		pthread_join(Students[D_id], NULL);

	//Freeing the allocated memory
	free(Students); 
	return 0;
}

void *Teachingasst_Activity()
{
	while(1)
	{
		sem_wait(&TeachingAsst_Sleep);		//TeachingAssistent is currently sleeping.
		printf("TeachingAssistent has been awaken.\n");

		while(1)
		{
			// locking
			pthread_mutex_lock(&ChairObtained);
			if(NoOfChairs == 0) 
			{
				//if the chairs are empty, break the loop.
				pthread_mutex_unlock(&ChairObtained);
				break;
			}
			//TeachingAssistent gets next student on the chair.
			sem_post(&ChairsSem[WorkingIndex]);
			NoOfChairs--;
			printf("Student leaves the chairs.chairs lefted are %d\n", 3 -NoOfChairs);
			WorkingIndex = (WorkingIndex + 1) % 3;
			pthread_mutex_unlock(&ChairObtained);
			// unlocking

			printf("\t TeachingAssistent is helping the student now.\n");
			sleep(5);
			sem_post(&Student_Sem);
			usleep(1000);
		}
	}
}
void *Student_Activity(void *threadID) 
{
	int ProgrammingTime;

	while(1)     //running the while loop for Student_Activity
	{
		printf("Student %ld is doing work.\n", (long)threadID);
		ProgrammingTime = rand() % 10 + 1;
		sleep(ProgrammingTime);		//Sleep for a random  amount of time period.

		printf("Student %ld needs  TeachingAssistent's help\n", (long)threadID);
		
		pthread_mutex_lock(&ChairObtained);
		int count = NoOfChairs;
		pthread_mutex_unlock(&ChairObtained);

		if(count < 3)		//Student tried to sit on a chair.
		{
			if(count == 0)		//If student sits on first empty chair, wake up the TeachingAssistent.
				sem_post(&TeachingAsst_Sleep);
			else
				printf("Student %ld sitting on the chair waiting for the TeachingAssistent to finish. \n", (long)threadID);

			// locking
			pthread_mutex_lock(&ChairObtained);
			int index = (WorkingIndex +NoOfChairs) % 3;
			NoOfChairs++;
			printf("Student sits on  the chair.Chairs Remaining: %d\n", 3 -NoOfChairs);
			pthread_mutex_unlock(&ChairObtained);
			// unlocking

			sem_wait(&ChairsSem[index]);		//Student leaves his/her chair.
			printf("\t Student %ld is getting help from the TeachingAssistent. \n", (long)threadID);
			sem_wait(&Student_Sem);		//Student waits to go next.
			printf("Student %ld left TeachingAssistent room.\n",(long)threadID);
		}
		else 
			printf("Student %ld will return at  some another time. \n", (long)threadID);
			//If student didn't find any chair to sit on.
	}
}

