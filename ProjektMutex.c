#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
//zdefiniowanie stalej wartosci maksymalnej liczby samochodow
#define MAX 100
//zmienna okreslajaca czy maja sie wyswietlac zawartosci kolejek
int info = false;
int onBridge = 0;
//inicjalizacja statycznego mutexa
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//deklaracja struktury wezla kolejki
typedef struct Queue {
    pthread_t thread;
    struct Queue* next;
} Queue;
//dodawanie nowego elementu kolejki
void insertQueue(Queue** head, const char* car) {
    //stworzenie nowego wezla
    Queue* newNode = malloc(sizeof(Queue));
    //przypisanie zmiennej nameCar zawartosci podanej na wejscie zmiennej car
    strncpy(newNode->nameCar, car, MAX - 1);
    //przypisanie \0 na koniec listy znakow i przypisanie kolejnemu wezlowi wartosci NULL
    newNode->nameCar[MAX - 1] = '\0';
    newNode->next = NULL;
    //sprawdzenie czy kolejka jest pusta
    if (*head == NULL) {
        *head = newNode;
    }
    else {
        Queue* current = *head;
        //przeiterowanie na koniec kolejki
        while (current->next != NULL) {
            current = current->next;
        }
        //wstawienie do kolejki wezla newNode na ostatniej pozycji
        current->next = newNode;
    }
}
//usuwanie pierwszego elementu kolejki
void popQueue(Queue** head) {
    //sprawdzanie czy kolejka jest pusta
    if (*head == NULL) {
        printf("Error: The queue is emptyP.\n");
        return;
    }
    //usuwanie przez przypisanie headowi wezla nastepnego
    Queue* temp = *head;
    *head = (*head)->next;
    free(temp);
}
//zwracanie nazwy
char* topQueue(Queue* head) {
    //sprawdzanie czy kolejka jest pusta
    if (head == NULL) {
        printf("Error: The queue is emptyT.\n");
        return NULL;
    }
    //zwracanie nazwy pierwszej pozycji w kolejce
    return head->thread;
}
//wypisywanie zawartosci kolejki
void printQueue(Queue* head) {
    printf("Queue:\n");
    //sprawdzenie czy kolejka jest pusta
    if (head == NULL) {
        printf("Empty.\n\n");
        return;
    }
    else {
        Queue* current = head;
        int i = 1;
        //iteracja po wezlach az do konca kolejki wypisujac ich zawartosc
        while (current != NULL) {
            printf("%d: %s\n", i, current->thread);
            current = current->next;
            i++;
        }
    }
    printf("\n");
    return;
}
//okreslenie wielkosci kolejki
int sizeQueue(Queue* head) {
    int size = 0;
    Queue* current = head;
    //zliczanie ilosci wezlow
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}
//losowe obliczenia
void cityWait()
{
    for (int i = 0;i < 1000000000;i++) {
        int b = i + 1;
    }
}
//watek miasta A pozwalajacy przemieszczenie sie pojazdu z kolejki queueA do kolejki queueB
void* city(void* arg) {
    int id = threadID;
    char whichCity = 'A';
    //nieskonczona petla watku
    while (1) {
        cityWait();
        pthread_mutex_lock(&mutex); //zablokowanie mutexa
        //sprawdzenie czy kolejka A jest pusta
        pritnf(">>");
        if (whichCity = 'A'){
            whichCity = B;
        }
        else {
            whichCity = A;
        }
        
        //sprawdzenie czy program jest odpalony z parametrem -info, potencjalne wypisanie zawartosci kolejek
       /* if (info == true) {
            printf("---\n");
            printQueue(*qA);
            printQueue(*qB);
        }
        //wypisanie wartosci wedlug schematu
        printf("CityA-%d QueueA-%d --> [>> %s >>] <-- QueueB-%d CityB-%d\n", count_CityA, sizeQueue(*qA), temp, sizeQueue(*qB), count_CityB);
        printf("CityA-%d QueueA-%d --> [<< %s <<] <-- QueueB-%d CityB-%d\n", count_CityA, sizeQueue(*qA), temp, sizeQueue(*qB), count_CityB);
        */
        pthread_mutex_unlock(&mutex); //odblokowanie mutexa
    }
    //wyjscie z watku
    pthread_exit(NULL);
}

//wyjscie z watku
pthread_exit(NULL);
}

//main
int main(int argc, char** argv) {
    //domyslna ilosc samochodow
    int cars = 5;
    //sprawdzanie ilosci argumentow
    if (argc < 1 || argc > 4) {
        printf("Invaid arguments. Try -N [Number of cars] or -info\n");
        return 1;
    }
    //sprawdzanie typow argumentow i wykonywanie odpowiednich zadan w zaleznosci od argumentu
    for (int i = 1; i < argc; i++) {
        //sprawdzanie czy pojawil sie argument -N i przypisanie nowej wartosci dla ilosci samochodow
        if (strcmp(argv[i], "-N") == 0) {
            i++;
            if (argv[i] == NULL || atoi(argv[i]) <= 0) {
                printf("Invalid number of cars. Insert value between 1 and %d\n", MAX);
                return 1;
            }
            cars = atoi(argv[i]);
        }
        else if (strcmp(argv[i], "-info") == 0) { //sprawdzanie czy pojawil sie argument -info
            info = 1;
        }
        else {
            printf("Invalid argument: %s\n", argv[i]);
            return 1;
        }
    }
    //wypisanie wartosci zmiennych cars i info
    fprintf(stdout, "info: %d\n", info);
    fprintf(stdout, "cars: %d\n", cars);
    printf("Starting Queue:\n");
    //oddzielenie
    printf("-----\n");
    //przypisanie pamieci watkom
    pthread_t* tid = malloc(cars * sizeof(pthread_t));  // Alokacja pamieci

    //stworzenie watkow
    for (int i = 0; i < cars; i++) {
        carID = i + 1;
        pthread_create(&tid[i], NULL, city, carID);
    }
    //oczekiwanie na zakonczenie watkow
    for (int i = 0; i < cars; i++) {
        pthread_join(tid[i], NULL);
    }
    //zwolnienie pamieci
    free(tid);
    free(queues);

    return 0;
}
