// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

int signal_received = 0;
int sleep_ns = 10; // 1 micro

void signal_handler(int sig)
{
    signal_received = 1;
    std::cout <<"Closing Application ..."<< std::endl;
}

unsigned long long get_cpuaffinity()
{
    unsigned i;
    uint64_t res;
    cpu_set_t cpuset;
    int rc = pthread_getaffinity_np(pthread_self(),sizeof(cpuset),&cpuset);
    if (rc) {
        errno = rc;
        return 0;
    }

    res = 0;
    for (i = 0; i < 64; ++i) {
        if (CPU_ISSET(i,&cpuset)) {
            res |= (1ULL << i);
        }
    }

    return res;
}
 
void foo() 
{
    cpu_set_t cpuset;
    pthread_t tid = pthread_self();
    
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset);
     
    pthread_setaffinity_np(tid, sizeof(cpuset), &cpuset);
      
    timespec time_val{}; 
    //time_val.tv_nsec = sleep_ns; // 10 micro sec
    
    // do stuff...
    for(int i = 0 ; !signal_received; i++)
    {
        std::cout << " foo: "<< tid<< std::endl;
        //nanosleep( &time_val, NULL);
    }
}

void bar(int x)
{
    cpu_set_t cpuset;
    pthread_t tid = pthread_self();
    
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
     
    pthread_setaffinity_np(tid, sizeof(cpuset), &cpuset);
    timespec time_val{}; 
    //time_val.tv_nsec = sleep_ns; // 10 micro sec
    // do stuff...
    for(int i = 0 ; !signal_received; i++)
    {
        std::cout << " bar: "<< tid << std::endl;
        //nanosleep( &time_val, NULL);
    }
}

int main() 
{
  
    std::cout << "Current affinity: " << get_cpuaffinity() 
              << "\nHIT ENTER ..."    << std::endl;
  
    std::string str;
    std::getline(std::cin, str, '\n'); 
    
    signal(SIGINT, &signal_handler);
    std::thread first (foo);     // spawn new thread that calls foo()
    std::thread second (bar,0);  // spawn new thread that calls bar(0)
 
    std::cout << "main, foo and bar now execute concurrently...\n";
 
    // synchronize threads:
    first.join();                // pauses until first finishes
    second.join();               // pauses until second finishes
 
    std::cout << "foo and bar completed.\n";
    
    int i = 0;
    for(; i < CPU_SETSIZE; i++)
    {
 
    }
    
    std::cout << "cpu size: " << i << std::endl;
 
    return 0;
}
