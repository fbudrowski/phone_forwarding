/** @file
 * Interfejs klasy przechowującej tablicę zmiennej długości.
 *
 * @author Franciszek Budrowski
 * @copyright Franciszek Budrowski
 * @date 2018/03/05
 *
 */




#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>



#ifndef WIELOMIANY_ARRAYLIST_H
#define WIELOMIANY_ARRAYLIST_H

#endif //WIELOMIANY_ARRAYLIST_H

/**
 * Struktura przechowująca element tablicy
 */

typedef struct ArrayListNode {
    size_t pos;
    bool valueToBeFreed;
    void *value;
} ArrayListNode;

/**
 * Struktura przechowująca tablicę zmiennej długości
 */

typedef struct PhoneNumbers {

    size_t arrSize;
    size_t phNumCount;
    ArrayListNode ** phNums;

} PhoneNumbers;

typedef PhoneNumbers ArrayList;

/**
 * Tworzy nową pustą tablicę
 * @return Wskaźnik na nową, pustą tablicę
 */
ArrayList* newArrayList (void);

/**
 * Tworzy nowy element tablicy ArrayList
 * @param Tekst jako char*.
 * @return Nowy element do ArrayList, zawierający kopię tekstu text
 */
ArrayListNode* newArrayListNode(char * text);

/**
 * Zwraca idx-ty (indeksowany od 0) element tablicy.
 * @param al - wskaźnik na tablicę
 * @param idx - pozycja, na którą wskazuje
 * @return Wskaźnik na zawartość idx-tego elementu tablicy
 */
void const * arrayListGet (ArrayList const * al, size_t idx);

/**
 * [ADAPTER] Zwraca idx-ty (indeksowany od 0) element tablicy.
 * @param al - wskaźnik na tablicę
 * @param idx - pozycja, na którą wskazuje
 * @return Wskaźnik na zawartość idx-tego elementu tablicy
 */
char const * phnumGet (PhoneNumbers const * pnum, size_t idx);

/**
 * Dodaje element na koniec tablicy
 * @param al - wskaźnik na tablicę
 * @param node - wskaźnik na element do dodania
 * @return Informacja, czy dodanie się udało
 */
bool arrayListAdd (ArrayList* al, ArrayListNode * node);

/**
 * Usuwa tablicę
 * @param al - tablica do usunięcia
 */
void arrayListDelete (ArrayList * al);

/**
 * [ADAPTER] Usuwa tablicę
 * @param pnum - tablica do usunięcia
 */
void phnumDelete (PhoneNumbers* pnum);

/**
 * Usuwa arrayListNode z tablicy, nie zachowuje kolejności. Jeżeli al jest NULLem, nic nie robi
 * @param al - tablica
 * @param aln - node do usunięcia
 */
void arrayListCutOut (ArrayList * al, ArrayListNode* aln);

/**
 * @param al - tablica do sprawdzenia
 * @return True jeżeli al jest NULLem lub pusta, false otherwise
 */
bool isArrayListEmpty (ArrayList * al);

/**
 * Sortuje tablicę char*-ek qsortem
 * @param al - tablica do posortowania
 */
void stringArrayListSort (ArrayList * al);