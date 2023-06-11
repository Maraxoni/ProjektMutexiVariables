#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100
int info = false;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int count_CityA = 5;
int count_CityB = 6;

typedef struct Queue {
    char samochod[MAX];
    struct Queue* next;
} Queue;

void insertQueue(Queue** head, const char* samochod) {
    Queue* newNode = malloc(sizeof(Queue));
    strncpy(newNode->samochod, samochod, MAX - 1);
    newNode->samochod[MAX - 1] = '\0';

    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        Queue* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void popQueue(Queue** head) {
    if (*head == NULL) {
        printf("Error: The list is emptyP.\n");
        return;
    }

    Queue* temp = *head;
    *head = (*head)->next;
    free(temp);
}

char* topQueue(Queue* head) {
    if (head == NULL) {
        printf("Error: The list is emptyT.\n");
        return NULL;
    }

    return head->samochod;
}

void printQueue(Queue* head) {
    printf("Queue:\n");
    if (head == NULL) {
        printf("Empty.\n\n");
        return;
    } else {
        Queue* current = head;
        int i = 1;
        while (current != NULL) {
            printf("%d: %s\n", i, current->samochod);
            current = current->next;
            i++;
        }
    }
    printf("\n");
    return;
}

int sizeQueue(Queue* head) {
    int size = 0;
    Queue* current = head;
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}

void* miasto_A(void* arg) {
    Queue** queues = *(Queue***)arg;
    Queue** qA = &queues[0];
    Queue** qB = &queues[1];

    while (1) {
        usleep(rand() % 1000000 + 100000); // od 0.1 do 1.1 sekundy
        pthread_mutex_lock(&mutex);
        if (*qA == NULL) {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        char temp[MAX];
        sprintf(temp, "%s", topQueue(*qA));
        popQueue(qA);
        if(info==true){
            printf("---\n");
            printQueue(*qA);
            printQueue(*qB);
            printf("MiastoA-%d KolejkaA-%d --> [>> %s >>] <-- KolejkaB-%d MiastoB-%d\n", count_CityA, sizeQueue(*qA), temp, sizeQueue(*qB), count_CityB);
        } else {
            printf("MiastoA-%d KolejkaA-%d --> [>> %s >>] <-- KolejkaB-%d MiastoB-%d\n", count_CityA, sizeQueue(*qA), temp, sizeQueue(*qB), count_CityB);
        }
        
        insertQueue(qB, temp);

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void* miasto_B(void* arg) {
    Queue** queues = *(Queue***)arg;
    Queue** qA = &queues[0];
    Queue** qB = &queues[1];

    while (1) {
        usleep(rand() % 1000000 + 100000); // od 0.1 do 1.1 sekundy
        pthread_mutex_lock(&mutex);
        if (*qB == NULL) {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        char temp[MAX];
        sprintf(temp, "%s",topQueue(*qB));
        popQueue(qB);
        if(info==true){
            printf("---\n");
            printQueue(*qA);
            printQueue(*qB);
            printf("MiastoA-%d KolejkaA-%d --> [<< %s <<] <-- KolejkaB-%d MiastoB-%d\n", count_CityA, sizeQueue(*qA), temp, sizeQueue(*qB), count_CityB);
        } else {
            printf("MiastoA-%d KolejkaA-%d --> [<< %s <<] <-- KolejkaB-%d MiastoB-%d\n", count_CityA, sizeQueue(*qA), temp, sizeQueue(*qB), count_CityB);
        }
        
        insertQueue(qA, temp);
        
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    int cars = 5;

    if (argc < 1 || argc > 4) {
        printf("Invaid arguments. Try -N [Number of cars] or -info\n");
        return 1; 
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-N") == 0) {
            i++;
            if (argv[i]==NULL || atoi(argv[i]) <= 0 ) {
                printf("Invalid number of cars. Insert value between 1 and %d\n", MAX);
                return 1; 
            }
            cars = atoi(argv[i]);
        } else if (strcmp(argv[i], "-info") == 0) {
            info = 1;
        } else {
            printf("Invalid argument: %s\n", argv[i]);
            return 1;
        }
    }
    
    

    fprintf(stdout,"Nowa wartosc zmiennej info: %d\n", info);
    fprintf(stdout,"Nowa wartosc zmiennej cars: %d\n", cars);

    Queue* queueA = NULL;
    Queue* queueB = NULL;
    Queue** queues = malloc(2 * sizeof(Queue*));

    for (int i = 0; i < cars; i++) {
        char car[MAX];
        sprintf(car, "Car%d", i + 1);
        insertQueue(&queueA, car);
    }
    printQueue(queueA);
    printQueue(queueB);
    queues[0] = queueA;
    queues[1] = queueB;

    pthread_t* tid1 = malloc(cars * sizeof(pthread_t));  // Poprawiony rozmiar alokacji
    pthread_t* tid2 = malloc(cars * sizeof(pthread_t));  // Poprawiony rozmiar alokacji


    for (int i = 0; i < cars; i++) {
        pthread_create(&tid1[i], NULL, miasto_A, &queues);
        pthread_create(&tid2[i], NULL, miasto_B, &queues);
    }

    for (int i = 0; i < cars; i++) {
        pthread_join(tid1[i], NULL);
        pthread_join(tid2[i], NULL);
    }

    free(tid1);
    free(tid2);
    free(queues);

    return 0;
}
