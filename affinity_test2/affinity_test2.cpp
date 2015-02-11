//#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>

void* pthread_Message(void* message)
{
    printf("%s is running on CPU %d\n", (char*)message, sched_getcpu());
    return NULL;
}

int main()
{
    pthread_t thread1, thread2, thread3, thread4;
    pthread_t threadArray[4];
    cpu_set_t cpu1, cpu2, cpu3, cpu4;
    char thread1Msg[] = "Thread 1";
    char thread2Msg[] = "Thread 2";
    char thread3Msg[] = "Thread 3";
    char thread4Msg[] = "Thread 4";
    int thread1Create, thread2Create, thread3Create, thread4Create, i, temp;

    CPU_ZERO(&cpu1);
    CPU_SET(1, &cpu1);
    temp = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu1);
    printf("Set returned by pthread_getaffinity_np() contained:\n");
    for (i = 0; i < CPU_SETSIZE; i++)
        if (CPU_ISSET(i, &cpu1))
            printf("CPU1: CPU %d\n", i);

    thread1Create = pthread_create(&thread1, NULL, pthread_Message, thread1Msg);

    //thread 2
    CPU_ZERO(&cpu2);
    CPU_SET(2, &cpu2);
    temp = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu2);
    for (i = 0; i < CPU_SETSIZE; i++)
        if (CPU_ISSET(i, &cpu2))
            printf("CPU2: CPU %d\n", i);

    thread2Create = pthread_create(&thread2, NULL, pthread_Message, thread2Msg);

    //thread3 
    CPU_ZERO(&cpu3);
    CPU_SET(3, &cpu3);
    temp = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu3);
    for (i = 0; i < CPU_SETSIZE; i++)
        if (CPU_ISSET(i, &cpu3))
            printf("CPU3: CPU %d\n", i);
    thread3Create = pthread_create(&thread3, NULL, pthread_Message, thread3Msg);

    //thread 4
    CPU_ZERO(&cpu4);
    CPU_SET(0, &cpu4);
    temp = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu4);
    for (i = 0; i < CPU_SETSIZE; i++)
        if (CPU_ISSET(i, &cpu4))
            printf("CPU4: CPU %d\n", i);
    thread4Create = pthread_create(&thread4, NULL, pthread_Message, thread4Msg);



    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    return 0;
}

