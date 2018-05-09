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
    size_t pos; /** Pozycja w tablicy. Jeżeli element nie jest w tablicy -- undefined. */
    bool valueToBeFreed; /** Czy element pod *value trzeba uwalniać przy delete */
    void *value; /** Element, na który wskazuje node */
} ArrayListNode;

/**
 * Struktura przechowująca tablicę zmiennej długości
 */

typedef struct PhoneNumbers {

    size_t arrSize; /** Obecna wielkość tablicy */
    size_t phNumCount; /** Liczba elementów w tablicy */
    ArrayListNode ** phNums; /** Tablica */

} PhoneNumbers;

typedef PhoneNumbers ArrayList;

/**
 * Tworzy nową pustą tablicę, lub zwraca NULL jeżeli się nie uda zalokować pamięci.
 * @return Wskaźnik na nową, pustą tablicę lub NULL
 */
ArrayList* newArrayList (void);

/**
 * Tworzy nowy element tablicy ArrayList. Jeżeli alokacja pamięci się nie powiedzie, zwraca NULL.
 * @param Tekst jako char*.
 * @return Nowy element do ArrayList, zawierający kopię tekstu text lub NULL.
 */
ArrayListNode* newArrayListNode(char * text);

/**
 * Zwraca idx-ty (indeksowany od 0) element tablicy lub NULL jeżeli al jest NULLem lub idx jest poza rozmiarem tablicy.
 * @param al - wskaźnik na tablicę
 * @param idx - pozycja, z której funkcja odzyskuje wartość
 * @return Wskaźnik na zawartość idx-tego elementu tablicy lub NULL.
 */
void const * arrayListGet (ArrayList const * al, size_t idx);

/**
 * [ADAPTER (arrayListGet)] Zwraca idx-ty (indeksowany od 0) element tablicy.
 * @param al - wskaźnik na tablicę
 * @param idx - pozycja w tablicy al
 * @return Wskaźnik na zawartość idx-tego elementu tablicy. Jeżeli tablica jest NULLem lub rozmiar tablicy <= idx, to zwraca NULL.
 */
char const * phnumGet (PhoneNumbers const * pnum, size_t idx);

/**
 * Dodaje element na koniec tablicy
 * @param al - wskaźnik na tablicę
 * @param node - wskaźnik na element do dodania
 * @return Informacja, czy dodanie się powiodło. W szczególności false, jeżeli tablica lub element jest NULLem.
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
 * Sortuje tablicę char*-ek qsortem. Nic nie robi, gdy tablica jest NULLem.
 * @param al - tablica do posortowania
 */
void stringArrayListSort (ArrayList * al);