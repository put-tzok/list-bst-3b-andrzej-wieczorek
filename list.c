#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = {400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800};

// each list node contains an integer key value and pointer to next list node
struct node {
    int key;
    struct node *next;
};

// list's beginning is called the head of list
struct node *head = NULL;


struct node *new_el; //globalny wskaznik do dodawania elementow

struct node* list_insert(int value) {
    new_el = malloc(sizeof(struct node)); // zaalokowanie pamiecina nowy wezel listy
    new_el->next = head;  // wskazanie na dotychcasowa glowe
    new_el->key = value;
    head = new_el;  // wyskazanie, ze nowy wzel jest, nowa glowa listy

    return head;//NULL
}

struct node* list_search(int value) {
    struct node *tmp = head;  // tymczasowe wskazanie glowy[do przesuwnia po liscie]

    // UWAGA NA WARUNEK WAZNE: || ...... !!! gdy poszukiwany element jest ostatnim
    while(tmp->next != NULL || tmp->key == value){
        if(tmp->key == value){  // warunek znalezienia
            return tmp;  // zwrocenie wskaznika na poszukiwany element
        }
        tmp = tmp->next;  // przesuniecie sie w liscie
    }
    return NULL;  // gdy element nie znaleziony
}

void list_delete(int value) {
    // gdy usuwamy glowe listy
    if(head->key == value){
        struct node *toDel; // zeby zwolic
        toDel = head;
        head = head->next;
        free(toDel);

        return ;
    }

    // dla pozostalych przypadkow
    struct node *tmp = head->next; // zaczynamy od 2 elementu listy, [dalej do wskazywanie aktuaalnego elementu]
    struct node *prev = head; // pierwszy element listy, wskaznik na poprzednika
    struct node *next;  // wskaznik na kolejny element

    while(tmp->next != NULL || tmp->key == value){
        if(tmp->key == value){
            //sprawdzenie czy nie ostatni element listy
            if(tmp->next == NULL){
                prev->next = NULL; // odciecie ostatniego elementu
                free(tmp);

                return ;
            }

            next = tmp->next;
            prev->next = next;
            free(tmp);

            return ;
        }

        tmp = tmp->next; // przsuniecie glownego wskjaznika
        prev = prev->next; // przesuniecie wskaznika na poprzednika
    }
}


// wielkosc listy jednokierunkowej
unsigned int list_size() {
    if(head == NULL){
        return 0; // jesli lista jest pusta
    }

    unsigned int size = 1;  // jesli ma korzen, rozmiar na pewno 1
    struct node *tmp = head;  // pomocniczy wskaznik do przejsica listy

    while(tmp->next != NULL){
        tmp = tmp->next;
        size++;
    }

    return size;
}

/*
 * Fill an array with increasing values.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void fill_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        t[i] = i;
    }
}

/*
 * Reorder array elements in a random way.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void shuffle(int *t, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % i;
        int temp = t[i];
        t[i] = t[j];
        t[j] = temp;
    }
}

int main() {
    FILE * data;
    data = fopen("data-list.txt", "w");
    bool no_yes[] = { false, true };

    for (unsigned int i = 0; i < sizeof(no_yes) / sizeof(*no_yes); i++) {
        bool enable_shuffle = no_yes[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

            // always create an array of size `n` and fill it with increasing values
            int *t = malloc(n * sizeof(*t));
            fill_increasing(t, n);

            // if true, reorder array elements randomly
            if (enable_shuffle) {
                shuffle(t, n);
            }

            // insert elements in the order present in array `t`
            clock_t insertion_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_insert(t[k]);
                assert(iter != NULL);       // inserted element cannot be NULL
                assert(iter->key == t[k]);  // inserted element must contain the expected value
            }
            insertion_time = clock() - insertion_time;

            // reorder array elements before searching
            shuffle(t, n);

            // search for every element in the order present in array `t`
            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_search(t[k]);
                assert(iter != NULL);       // found element cannot be NULL
                assert(iter->key == t[k]);  // found element must contain the expected value
            }
            search_time = clock() - search_time;

            // reorder array elements before deletion
            shuffle(t, n);

            // delete every element in the order present in array `t`
            for (unsigned int k = 0, l = n; k < n; k++, l--) {
                assert(list_size() == l);   // list size must be equal to the expected value
                list_delete(t[k]);
            }
            assert(list_size() == 0);       // after all deletions, the list size is zero
            assert(head == NULL);           // after all deletions, the list's head is NULL

            free(t);

            fprintf(data,
                    "%d\t%s\t%f\t%f\n", n, enable_shuffle ? "true" : "false",
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
        }
    }
    fclose(data);
    return 0;
}
