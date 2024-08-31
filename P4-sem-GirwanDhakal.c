/*
Name:Girwan Dhakal
CWID:12274204
This programm uses semaphores and locks to implement the solution.
*/


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytime.h"
#include <semaphore.h>

int num_student, num_chair, left_interval, right_interval, num_waiting, num_extra;
sem_t sem_teacher, sem_student;
pthread_mutex_t  lock1 = PTHREAD_MUTEX_INITIALIZER; 

void *student_thread(void* args)
{
    if(num_chair == 0)
    {
        return NULL;
    }
    int student_tid = *(int*) args;
    int randomTime;
    printf("Student <%d> to sleep 1 second \n",student_tid, randomTime);
    sleep(1); 
    printf("Student <%d> wake up\n",student_tid);


    int help_count = 0;

    while(help_count < 2)
    {
        int choice = rand() % 2; // 0 is study, 1 is seek help
        if(choice == 0)
        {
            randomTime = mytime(left_interval, right_interval);
            printf("Student <%d> to sleep %d seconds\n",student_tid, randomTime);
            sleep(randomTime); //studying for a random time
            printf("Student <%d> wake up\n",student_tid);

        }
        else
        {
            // seeking help
            if(num_waiting == num_chair) // if the chairs are full
            {
                randomTime = mytime(left_interval, right_interval);
                printf("Student <%d> to sleep %d seconds\n",student_tid, randomTime);
                sleep(randomTime); //studying for a random time
                printf("Student <%d> wake up\n",student_tid);
            }
            else // chairs are not all full
            {
            printf("Student <%d> will call mutex_lock on lock1\n", student_tid);
            pthread_mutex_lock(&lock1);
            num_waiting++;
            printf("Student <%d> will call mutex_unlock on lock1\n", student_tid);
            pthread_mutex_unlock(&lock1);

            printf("Student <%d> will call sem_post on sem_student\n", student_tid);
            sem_post(&sem_student); // signals for help to teacher

            printf("Student <%d> will call sem_wait on sem_teacher\n", student_tid);
            sem_wait(&sem_teacher); // wait for help
            help_count++; //after help increment help coount
            }
        }
    }
    return NULL;
}

void *teacher_thread(void* args)
{
    int randomTime;
    pthread_t teacher_tid = pthread_self();
    printf("Teacher <%lu> to sleep 1 second \n",teacher_tid);
    sleep(1); 
    printf("Teacher <%lu> wake up\n",teacher_tid);
    while(1)
    {
        if(num_waiting > 0) // students are waiting
        {   
            printf("Teacher <%lu> will call sem_wait on sem_student\n", teacher_tid);
            sem_wait(&sem_student);

            randomTime = mytime(left_interval, right_interval);
            printf("Teacher <%lu> to sleep %d seconds\n",teacher_tid, randomTime);
            sleep(randomTime); //studying for a random time
            printf("Teacher <%lu> wakeup\n",teacher_tid);

            printf("Teacher <%lu> will call mutex_lock on lock1\n", teacher_tid);
            pthread_mutex_lock(&lock1);
            num_waiting--;
            printf("Teacher <%lu> will call mutex_lock on lock1\n", teacher_tid);
            pthread_mutex_unlock(&lock1);

            printf("Teacher <%lu> will call sem_post on sem_teacher\n", teacher_tid);
            sem_post(&sem_teacher); //signals that teacher is free because student got helped
        }
        else // no students are waiting teacher does his own work
        {
            printf("Teacher <%lu> will call sem_wait on sem_student\n", teacher_tid);
            sem_wait(&sem_student); // wait for students, does own work
        }
    }
}

 int main(int argc, char *argv[]) {
    //setup
    sem_init(&sem_student, 0, 0);
    sem_init(&sem_teacher, 0, 1);
    num_waiting = 0;
    if (argc != 5) {
		fprintf(stderr, "usage: %s <number of student> <number of chairs> <left interval> <right interval>\n", argv[0]);
		exit(1);
    }
	num_student = atoi(argv[1]);
    num_chair = atoi(argv[2]);
    left_interval = atoi(argv[3]);
    right_interval = atoi(argv[4]);
    pthread_t student_threads[num_student];
    int student_tids[num_student];

    //create teacher thread
    pthread_t teacherThread;
    pthread_create(&teacherThread, NULL, teacher_thread, NULL);

    //create student threads
    int i;
    for(i = 0; i < num_student; i++)
    {
        student_tids[i] = i;
        pthread_create(&student_threads[i], NULL, student_thread, (void*)&student_tids[i]);
    }


    // join the threads
    for(i = 0; i < num_student; i++)
    {
        pthread_join(student_threads[i], NULL);
    }
    //cleanup
    pthread_cancel(teacherThread);
    pthread_mutex_destroy(&lock1);
    sem_destroy(&sem_student);
    sem_destroy(&sem_teacher);

   return 0;
 }


