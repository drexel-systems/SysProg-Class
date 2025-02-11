#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

#define NUM_THREADS 10
#define MAX_COUNT 100

// Mutex for thread synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Shared resource
int shared_counter = 0;

// Structure to pass data to threads
typedef struct {
    int thread_id;
    int sleep_time;
} thread_data_t;

//posix reccomended sleep
void sleep_ms(int ms){
    struct timespec ts;
    ts.tv_sec = 0;  // 0 seconds
    ts.tv_nsec = ms * 1000000; // ms milliseconds
    nanosleep(&ts, NULL);
}

unsigned long get_task_id(){
    unsigned long tid = syscall(SYS_gettid);
    unsigned int pid = getpid();

    return (unsigned long)((tid << 32) | pid);
}

// Thread function
void* thread_function(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    unsigned long task_id = get_task_id();
    
    for (int i = 0; i < MAX_COUNT; i++) {
        printf("I am thread %d with taskid %8lx\n",
            data->thread_id, task_id);
        sleep(1);
    }
    
    printf("Thread %d: Finished execution\n", data->thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    
    printf("Main: Starting thread creation\n");
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].sleep_time = 1;  // Different sleep times
        
        int rc = pthread_create(&threads[i], NULL, 
                              thread_function, &thread_data[i]);
        
        if (rc) {
            printf("Error creating thread %d\n", i);
            exit(-1);
        }
        
        printf("Main: Created thread %d\n", i);
    }
    
    // Wait for all threads to complete
    printf("sleeping for 5 seconds\n");
    sleep(5);
    printf("Main: Program completed\n");
    printf("Everything should be gone for pid=%d\n", getpid());
    
    return 0;
}