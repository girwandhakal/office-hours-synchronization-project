/*
Name:Girwan Dhakal
CWID:12274204
This programm uses condition variables and locks to implement the solution.
*/


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytime.h"
#include <semaphore.h>

int num_student, num_chair, left_interval, right_interval, studentWaiting, helped;
pthread_mutex_t  lock1 = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t   cond_student = PTHREAD_COND_INITIALIZER; //to indicate student is waiting
pthread_cond_t   cond_help = PTHREAD_COND_INITIALIZER; //to indicate wether student was helped

void *student_thread(void* args)
{
    if(num_chair == 0) // no chairs then just end the thread
    {
        return NULL;
    }
    int student_tid = *(int*) args;
    int randomTime;

    printf("Student <%d> to sleep 1 second \n",student_tid, randomTime); // sleep after being created
    sleep(1); 
    printf("Student <%d> wake up\n",student_tid);


    int help_count = 0;
    while(help_count < 2) // each student gets help twice
    {

        int choice = rand() % 2; // 0 is study, 1 is seek help
        if(choice == 0)
        {
            randomTime = mytime(left_interval, right_interval);
            printf("Student <%d> to sleep %d seconds\n",student_tid, randomTime);
            sleep(randomTime); //studying for a random time
            printf("Student <%d> wake up\n",student_tid);
        }
        else // seeks help
        {
            printf("Student <%d> will call mutex_lock on lock1\n", student_tid);
            pthread_mutex_lock(&lock1);

            if(studentWaiting == num_chair) // if the chairs are full comeback later
            {
                randomTime = mytime(left_interval, right_interval);
                printf("Student <%d> will call mutex_lock on lock1\n", student_tid);
                pthread_mutex_unlock(&lock1);

                printf("Student <%d> to sleep %d seconds\n",student_tid, randomTime);
                sleep(randomTime); 
                printf("Student <%d> wake up\n",student_tid);
            }
            else
            {
                studentWaiting++; // increment student on chairs

                printf("Student <%d> will call cond_signal on cond_student\n", student_tid);
                pthread_cond_signal(&cond_student); // send signal that student is waiting

                while(helped == 0)
                {
                    printf("Student <%d> will call cond_wait on cond_help\n", student_tid);
                    pthread_cond_wait(&cond_help, &lock1);
                }
                helped = 0;

                printf("Student <%d> will call mutex_unlock on lock1\n", student_tid);
                pthread_mutex_unlock(&lock1);

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

    printf("Teacher <%lu> to sleep 1 second \n",teacher_tid, randomTime);
    sleep(1); 
    printf("Teacher <%lu> wake up\n",teacher_tid);
    while(1)
    {
        printf("Teacher <%lu> will call mutex_lock on lock1\n", teacher_tid);
        pthread_mutex_lock(&lock1);

        while(studentWaiting == 0) {  // teacher is doing own work
            printf("Teacher <%lu> will call cond_wait on cond_student\n", teacher_tid);
            pthread_cond_wait(&cond_student, &lock1); // Wait for student to signal
        }
        printf("Teacher <%lu> will call mutex_unlock on lock1\n", teacher_tid);
        pthread_mutex_unlock(&lock1);

        // simulate helping
        randomTime = mytime(left_interval, right_interval);
        printf("Teacher <%lu> to help student and sleep %d seconds\n", teacher_tid, randomTime);
        sleep(randomTime); 
        printf("Teacher <%lu> wakeup\n", teacher_tid);

        printf("Teacher <%lu> will call mutex_lock on lock1\n", teacher_tid);
        pthread_mutex_lock(&lock1);
        studentWaiting--; // decrement student waiting on chair
        helped = 1;

        printf("Teacher <%lu> will call cond_signal on cond_help\n", teacher_tid);
        pthread_cond_signal(&cond_help); // signal that student is done being helped

        printf("Teacher <%lu> will call mutex_unlock on lock1\n", teacher_tid);
        pthread_mutex_unlock(&lock1);
    }
    return NULL;
}

 int main(int argc, char *argv[]) {
    //setup
    helped = 0;
    studentWaiting = 0;
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
    pthread_cond_destroy(&cond_student);
    pthread_cond_destroy(&cond_help);
    pthread_mutex_destroy(&lock1);


   return 0;
 }

