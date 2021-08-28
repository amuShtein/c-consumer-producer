#include <stdio.h>
#include <pthread.h>

#define WAIT 5

int lock = 0;

pthread_mutex_t mutex;
pthread_cond_t condc, condp;

extern void enter_region(int* lock);
extern void leave_region(int* lock);

#define N 100
int buffer[N];

int insertion_index = 0, retrieving_index = 0, active = 0;

int counter = 0;

int produce_item() {
    for(int j = 0; j < WAIT; j++);
    counter++;
    return counter;
}

void consume_item(int item) {
//    for(int j = 0; j < 10000; j++) {
//        printf("log...\n");
//    }

    printf("[consume item: %d]\n", item);
}

void insert_item(int item) {

//    enter_region(&lock);
    pthread_mutex_lock(&mutex);
    if (active == N) {
        pthread_cond_wait(&condp, &mutex);
    }
    buffer[insertion_index] = item;
    insertion_index = (insertion_index + 1) % N;
    active = active + 1;
    printf("[insert item %d]\n", item);
    pthread_cond_signal(&condc);
    pthread_mutex_unlock(&mutex);
//    leave_region(&lock);
}

_Noreturn void* producer(void* arg) {
    int item;

    while(1) {
        item = produce_item();
        insert_item(item);
    }
}

int retrieve() {

    //    enter_region(&lock);
    pthread_mutex_lock(&mutex);
    if (active == 0) {
//        printf("waiting for item...\n");
        pthread_cond_wait(&condc, &mutex);
    }

    int item = buffer[retrieving_index];
    retrieving_index = (retrieving_index + 1) % N;
    active = active - 1;
    printf("[item: %d, active = %d, insertion = %d, retrieve = %d]\n", item, active, insertion_index, retrieving_index);
    pthread_cond_signal(&condp);
    pthread_mutex_unlock(&mutex);
//    leave_region(&lock);
    return item;
}

_Noreturn void* consumer(void* arg) {
    int item;

    while(1) {
        item = retrieve();
        consume_item(item);
    }
}

int main() {
    printf("Hello, World!\n");

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condc, NULL);
    pthread_cond_init(&condp, NULL);

    pthread_t consumer_tid, producer_tid;

    pthread_create(&producer_tid, NULL, producer, NULL);
    pthread_create(&consumer_tid, NULL, consumer, NULL);

    void* ret;
    pthread_join(producer_tid, &ret);
    pthread_join(consumer_tid, &ret);

    return 0;
}
