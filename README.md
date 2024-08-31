# Office Hours Synchronization Project


## Problem Description:
A teacher sets up office hours to answer questions relating to course materials and assignments. The teacher meets one student at a time. There are n chairs outside the office where students can sit and wait if the teacher is currently helping
another student. The teacher helps students in a first-come-first-serve way. The interactions between students and the
teachers are possibly in four cases: 
    (1) When there are no students who need help during office hours, the teacher goes
back to own work. 
    (2) If a student arrives during office hours and finds the teacher on own work, the student raises the
teacher attention (which the teacher will respond) to ask for help. 
    (3) If a student arrives and finds the teacher currently
helping another student, the student sits on one of the chairs and waits. 
    (4) If no chairs are available, the student will
go study and come back at a later time. 

- **Project 1:** Uses condition variables and mutex locks.
- **Project 2:** Uses semaphores and mutex locks.


## Project 1: Condition Variables and Mutex Locks

This implementation uses condition variables (`pthread_cond_t`) and mutex locks (`pthread_mutex_t`) to synchronize the interactions between students and the teacher.

### Key Features

- **Condition Variables:** Used to signal when a student is waiting (`cond_student`) and when the student has been helped (`cond_help`).
- **Mutex Locks:** Used to protect shared resources (`lock1`).
- **First-Come, First-Serve:** Ensures students are helped in the order they arrive.

### Files

- `project1.c`: Contains the implementation using condition variables and mutex locks.
- `mytime.c` : Contains helper function to generate random time intervals.

### How to Run

```bash
gcc project1.c mytime.c mytime.h -pthread
./project1 <number_of_students> <number_of_chairs> <left_interval> <right_interval>
```


## Project 2: Semaphores and Mutex Locks

This implementation uses semaphores (`sem_t`) and mutex locks (`pthread_mutex_t`) to synchronize the interactions between students and the teacher.

### Key Features

- **Semaphores:** Used to signal when a student is waiting (`sem_student`) and when the teacher is available (`sem_teacher`).
- **Mutex Locks:** Used to protect shared resources (`lock1`).
- **Semaphore-Based Signaling:** Provides an alternative approach to using semaphores for synchronization, allowing for efficient signaling between threads.

 
