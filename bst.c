#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

unsigned int ns[] = { 100, 200, 400, 800, 1600, 3200, 6400, 12800};

// each tree node contains an integer key and pointers to left and right children nodes
struct node {
    int key;
    struct node *left;
    struct node *right;
};

// tree's beginning is called the root
struct node *root = NULL;


struct node **tree_search(struct node **candidate, int value) {
    struct node **result = candidate; // pomocniczy wskaznik

    if(*result == NULL){
        //printf("TREE SEARCH \n");
        return result;
    }

    if(value < (*result)->key){ //jesli mozna się jeszczxe zagłębić
        return tree_search(&((*result)->left), value); //rekurencyjne wywalane dla lewego poddrzewa
    }

    if(value > (*result)->key){ // jesli mozna się jeszczxe zagłębić
            return tree_search(&((*result)->right), value); // rekurencyjne wywalane dla prawgo poddrzewa
    }
    //printf("TREE SEARCH \n");
    return result; // gdy znajdziemy

    }

//-------------- funkcja tworzacac i inicjalizujaca nowy wezel
struct node *tmp; // pomocniczy wskaznik do alokowania pamieci pod wezel

void create_node(struct node **candidate, int value){
    tmp = malloc(sizeof(struct node));  // alokacja pamieci wezla
    // inicjalizowanie wartosci dla wezla i NULL dla potomkow
    tmp->key = value;
    tmp->left = NULL;
    tmp->right = NULL;

    *candidate = tmp; // podlaczenie nowego wezla pod odpowiednie miejsca w BST
    //printf("CREATE NODE \n");
}

struct node *tree_insert(int value){
    //printf("TREE INSERT \n");
    struct node **nodeptr;
    nodeptr = tree_search(&root, value); // wyszukanie miejsca

    //printf("TREE INSERT \n");
    create_node(nodeptr, value); // wywolanie stworznia wezla

    return *nodeptr; // zwrocenie wskaznika na dodany wezel
}

struct node **tree_maximum(struct node **candidate) {
    if((**candidate).right != NULL){
           return tree_maximum(&(**candidate).right);
        }

    return candidate;
}

void tree_delete(int value) {
    struct node **candidate = tree_search(&root, value);              //wyszukanie węzła
    if ((*candidate)->left == NULL && (*candidate)->right == NULL){   //liść w drzewie
        *candidate = NULL;
    }
    else if (((*candidate)->left != NULL) && ((*candidate)->right == NULL))    //usuwany element jeden lewy potomek
    {                                                                          //uwaga na referencje
        *candidate = (*candidate)->left;                                       //wskaźnik wskazujący usuwaną liczbę wkazuje teraz na jego potomka
    }
    else if (((*candidate)->left == NULL) && ((*candidate)->right != NULL))    //analogicznie dla jednego prawego
    {
        *candidate = (*candidate)->right;
    }
    else                                                                       //gdy 2 potomków
    {
        struct node **maxcandidate;
        maxcandidate = tree_maximum(&(*candidate)->left);                      //szukam wartości maksymalnej w lewym poddrzewie
        (*candidate)->key = (*maxcandidate)->key;                              //wartość usuwanego elementu jest zastępowana tym maxem
        *maxcandidate = (*maxcandidate)->left;                                 //jeśli max z lewego poddrzewa miał potomka po lewej(mniejszy)
    }                                                                          //zajmuje miejsce tego wezla
}


/* TEST =================================================================================================================
// w lewym podrzewie dotarcie do skranie prawgo wezla(element wikszy niz wszystkie
// niz wszystkie pozostale w lewym poddrzeiw i mnijszy niz elementy w prawym)

struct node **tree_left_max(struct node **candidate){
        if((*candidate)->right != NULL){
            return tree_left_max(&((*candidate)->right)); // zaglebianie sie w drzewie
        }

        return candidate; // gdy dotrze do wskazanej wartosci
}

struct node **tree_right_min(struct node **candidate){
        if((*candidate)->left != NULL){
            return tree_right_min(&((*candidate)->left)); // zaglebianie sie w drzewie
        }

        return candidate; // gdy dotrze do wskazanej wartosci

}

struct node **tree_maximum(struct node **candidate) {
    printf("tree_maximum \n");
    if((*candidate)->left != NULL){
        return tree_left_max(&((*candidate)->left)); // zaglebianie sie w drzewie lewym poddrzewie po max
    } else if((*candidate)->right != NULL) { // jesli nie istnije lewe poddrzewo
        return tree_right_min(&((*candidate)->right)); // zaglebianie sie w drzewie prawy poddrzewie po min
    }

    return NULL; // gdyby taka wartosc nie istniała
}

// usuwanie z uwzgledniniem 3 mozliwych przypadkow
void tree_delete(int value) {
    struct node **tmp;  // pomocnicza zmienna do usuniecia wezla
    struct node **tmp_sub; // pomoicnicza do innych wezlow

    printf("tree_delete \n");

    //usuwanie korzenia
    if(root->key == value){
        printf("korzenia \n");
        tmp = tree_maximum(&root); // wyszukanie elementu do podmiany
        // zaleznie czy podany element jest z lewego czy prawgo poddrzewa
        if((*tmp)->key > root->key){
            //(*tmp)->left = root->left;
            root->right = (*tmp)->right;

        } else {
            (*tmp)->right = root->right;
            root->left = (*tmp)->left;
        }

        root->key = (*tmp)->key;
        free(*tmp);  // usuniecie dotychczasowej pozycji wezla

        return;
    }


    // wyszukanie pozstałych węzłów
    tmp = tree_search(&root, value);  // wyszukanie wezla o wskazanej warosci
    tmp_sub = tree_maximum(tmp);

    if(tmp_sub == NULL){ // nie ma żadnego węzła poniżej
         printf("lisc \n");
        free(tmp);
        return;
    } else if((*tmp_sub)->key > (*tmp)->key){
        printf("inny prwao \n");
        //(*tmp_sub)->left = (*tmp)->left;
        printf("%d %d \n", (*tmp)->key, (*tmp_sub)->key);
        (*tmp)->right = (*tmp_sub)->right;
        (*tmp)->key = (*tmp_sub)->key;
        //*tmp = *tmp_sub;
        free(*tmp_sub);
        return;

    } else {
        printf("inny prwao \n");
        printf("%d %d \n", (*tmp)->key, (*tmp_sub)->key);
        //(*tmp_sub)->right = (*tmp)->right;
        (*tmp)->left = (*tmp_sub)->left;
        (*tmp)->key = (*tmp_sub)->key;
        //*tmp = *tmp_sub;
        free(*tmp_sub);
        return;
    }
}
========================================================================================================= */


unsigned int tree_size(struct node *element) {
    if(element == NULL){
        return 0; // nie zwieksznie rozmiaru i zaprzestanie zaglwbiania sie w drzwo
    }

    // rekurencyjne przejscie przez drzewo, by zliczyc wzly
    return 1 + tree_size(element->left) + tree_size(element->right);
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

/*
 * Check if tree is a valid BST.
 *
 * Parameters:
 *      struct node *element:   pointer to node to be checked
 *
 * Returns:
 *      bool:                   true if subtree rooted in "element" is a BST
 */
bool is_bst(struct node *element) {
    // empty tree is a valid BST
    if (element == NULL) {
        return true;
    }
    // leaf node is valid
    if (element->left == NULL && element->right == NULL) {
        return true;
    }
    // only right subnode? check it recursively
    if (element->left == NULL && element->right != NULL) {
        return (element->key < element->right->key) && is_bst(element->right);
    }
    // only left subnode? check it recursively
    if (element->left != NULL && element->right == NULL) {
        return (element->key > element->left->key) && is_bst(element->left);
    }
    // both subnodes present? check both recursively
    return (element->key > element->left->key)
        && (element->key < element->right->key)
        && is_bst(element->left)
        && is_bst(element->right);
}

void insert_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

void insert_random(int *t, int n) {
    shuffle(t, n);
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

//---------------------------------------------------------------------------
//SORTING ALGORITHM - QUICK SORT
//---------------------------------------------------------------------------
void swap(int *t, int i, int j)
{
    int tmp = t[i];
    t[i] = t[j];
    t[j] = tmp;
}

int partition(int *t, int p, int r)
{
    int x, i;
    x = t[r];
    i = p - 1;

    for(int j = p; j <= r - 1; j++)
    {
        if(t[j] < x)
        {
            i = i + 1;
            swap(t, i, j);
        }
    }

    i = i + 1;
    swap(t, i, r);

    return i;
}

int random_partition(int *t, int p, int r)
{
    int i = p + (rand() % (r - p));
    swap(t, i, r);
    return partition(t, p, r);

}

void quick_sort(int *t, int p, int r)
{
    int q;

    if(p < r)
    {
        //q = partition(t, p, r);
        q = random_partition(t, p, r);
        quick_sort(t, p, q - 1);
        quick_sort(t, q + 1, r);
    }
}

/*
void quick_sort(int *t, unsigned int n)
{
    quick_sort(t, 0, n - 1);
}
*/

//---------------------------------------------------------------------------
//SORTING ALGORITHM - QUICK SORT
//---------------------------------------------------------------------------

void insert_binary_bijection(int *t, unsigned int p, unsigned int r){
    unsigned int q;

    if(p == r){
        tree_insert(t[p]);
    } else if(r - p == 1) {
        tree_insert(t[p]);
        tree_insert(t[r]);
    } else {
        q = p + (r -p) / 2;
        tree_insert(t[q]); // dodanie el srodkowego
        insert_binary_bijection(t, p, q-1);
        insert_binary_bijection(t, q+1, r);

    }

}

void insert_binary(int *t, int n) {
    quick_sort(t, 0, n - 1);
    insert_binary_bijection(t, 0, n -1);
}

char *insert_names[] = { "Increasing", "Random", "Binary" };
void (*insert_functions[])(int*, int) = { insert_increasing, insert_random, insert_binary };

int main(int argc, char **argv) {
    //printf("START \n");

    FILE * data;
    data = fopen("data-bst.txt", "w");
    for (unsigned int i = 0; i < sizeof(insert_functions) / sizeof(*insert_functions); i++) {
        void (*insert)(int*, int) = insert_functions[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

            // crate an array of increasing integers: 0, 1, 2, ...
            int *t = malloc(n * sizeof(*t));
            fill_increasing(t, n);
            //printf("TEST1 \n");

            // insert data using one of methods
            clock_t insertion_time = clock();
            insert(t, n);
            insertion_time = clock() - insertion_time;


            assert(tree_size(root) == n);       // after all insertions, tree size must be `n`
            assert(is_bst(root));               // after all insertions, tree must be valid BST

            // reorder array elements before searching
            shuffle(t, n);


            // search for every element in the order present in array `t`
            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node **pnode = tree_search(&root, t[k]);
                struct node *iter = *pnode;
                assert(iter != NULL);           // found element cannot be NULL
                assert(iter->key == t[k]);      // found element must contain the expected value
            }
            search_time = clock() - search_time;

            // reorder array elements before deletion
            shuffle(t, n);

            // delete every element in the order present in array `t`
            for (unsigned int l = 0, m = n; l < n; l++, m--) {
                //printf("%d %d\n", l, t[l]);
                assert(tree_size(root) == m);   // tree size must be equal to the expected value
                tree_delete(t[l]);
                assert(is_bst(root));           // after deletion, tree must still be valid BST
            }
            assert(tree_size(root) == 0);       // after all deletions, tree has size zero

            free(root);
            free(t);

            fprintf(data,
                    "%d\t%s\t%f\t%f\n",
                    n,
                    insert_names[i],
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
        }
    }
    fclose(data);
    return 0;
}
