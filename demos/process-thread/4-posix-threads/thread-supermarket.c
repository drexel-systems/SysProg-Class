#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdatomic.h>

#define NUM_CUSTOMERS 10
#define NUM_CHECKOUTS 3

sem_t checkout_lines;
pthread_mutex_t reg_mutex = PTHREAD_MUTEX_INITIALIZER;
int registers[NUM_CHECKOUTS] = {1, 2, 3};  // Stack of available registers
int top = NUM_CHECKOUTS - 1;  // Stack pointer for available registers

// Function to get a free register
int get_register() {
    pthread_mutex_lock(&reg_mutex);
    int reg_id = registers[top--];  // Pop from stack
    pthread_mutex_unlock(&reg_mutex);
    return reg_id;
}

// Function to release a register
void release_register(int reg_id) {
    pthread_mutex_lock(&reg_mutex);
    registers[++top] = reg_id;  // Push back to stack
    pthread_mutex_unlock(&reg_mutex);
}

void* customer(void* arg) {
    int id = *(int*)arg;

    printf("Customer %d is waiting for a register...\n", id);

    // Wait for an available checkout line
    sem_wait(&checkout_lines);

        // Safely assign a register
        int reg_id = get_register();

        printf("Customer %d is checking out at register %d.\n", id, reg_id);
        
        sleep(rand() % 3 + 1);  // Simulate checkout time

        printf("Customer %d has finished at register %d.\n", id, reg_id);

        // Release the register for the next customer
        release_register(reg_id);

    // Signal that a checkout line is available
    sem_post(&checkout_lines);

    return NULL;
}

int main() {
    pthread_t customers[NUM_CUSTOMERS];
    int cust_ids[NUM_CUSTOMERS];

    // Initialize the semaphore with 3 available checkout lines
    sem_init(&checkout_lines, 0, NUM_CHECKOUTS);

    // Create customer threads
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        cust_ids[i] = i;
        pthread_create(&customers[i], NULL, customer, &cust_ids[i]);
        usleep(100000);  // Stagger thread creation slightly
    }

    // Wait for all customers to finish
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }

    // Destroy the semaphore
    sem_destroy(&checkout_lines);

    return 0;
}
