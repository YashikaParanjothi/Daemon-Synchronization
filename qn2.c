#include <iostream>
using namespace std;
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>

int cars;
int m; 
int waiters;
pthread_mutex_t lock;
sem_t waitingToGo;

void ArriveSaffari();                                                                                                                                                                                                                                                                                                                                                                                                                                                               void ExitBridge();
void ExitSaffari();

void *vehicle( void * arg)
{
        time_t t;
        int *id = (int *) arg;
        srand((unsigned) time(&t));
        int i;
        int Direction;

        cout << "Person-"<< *id << " has arrived in the park............." << endl;                                             
        sleep(1);                           // rest for a while
        cout <<"Person-"<< *id
            << "is ready to board the car "<< endl;
        ArriveSaffari();         // arrive at the queue

        cout <<"Person-"<< *id
            << " has boarded the car......" << endl;
        sleep(2);                           

        ExitSaffari();                           // exit the saffari
        cout <<"Person-"<< *id
            << " has left the car" << endl;
        

        cout <<"Person-"<< *id << " has left the park." << endl;
}

int isSafe()
{
        if(cars == m)                             //Make the thread wait if number of cars being run is value of m
        {
                return 1;
        }
        else
        {
                return 0;
        }
}


void ArriveSaffari()
{
        pthread_mutex_lock(&lock);
        // while cars not available, wait

        while (isSafe())
        {
                waiters++;
                sem_wait(&waitingToGo);
                waiters--;
        }

        // get on the car
        cars++;


        pthread_mutex_unlock(&lock);
}


void ExitSaffari()
{
        pthread_mutex_lock(&lock);

        // get off the car
        cars--;

        // if anybody wants to take the car, wake them
        if (waiters > 0)
        {
                sem_post(&waitingToGo);
        }

        pthread_mutex_unlock(&lock);
}


int main(int argc,char *argv[])
{
        int i;
        int threads;
        int j[50];
        pthread_t t_id[50];
        sem_init(&waitingToGo,0,1);
        threads = abs(atoi(argv[1]));
        m = abs(atoi(argv[2]));
        for(i=0;i<threads;i++)          //creating a thread for each person
        {
                j[i]= i + 1;
                sleep(1);
                pthread_create(&t_id[i], NULL, vehicle,(void *) &j[i]);
        }
        for( i = 0;i<threads; i++)
        {
                pthread_join(t_id[i], NULL);
        }
        pthread_mutex_destroy(&lock);
}
