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
    /** Pozycja w tablicy. Jeżeli element nie jest w tablicy -- undefined. */
    size_t pos;
    /** Stwierdza, czy element pod *value trzeba uwalniać (za pomocą free) przy delete */
    bool valueToBeFreed;
    /** Element, na który wskazuje node */
    void *value;
} ArrayListNode;

/**
 * Struktura przechowująca tablicę zmiennej długości
 */

typedef struct PhoneNumbers {

    /** Obecna wielkość tablicy */
    size_t arrSize;
    /** Liczba elementów w tablicy */
    size_t phNumCount;
    /** Tablica */
    ArrayListNode ** phNums;

} PhoneNumbers;

/** Właściwa nazwa dla PhoneNumbers - obie używamy zamiennie. */
typedef PhoneNumbers ArrayList;

/**
 * Tworzy nową pustą tablicę, lub zwraca NULL jeżeli się nie uda zalokować pamięci.
 * @return Wskaźnik na nową, pustą tablicę lub NULL
 */
ArrayList* newArrayList (void);


/**
 * Tworzy nowy element tablicy ArrayList. Używa wskaźnika @p ptr do nowego elementu tablicowego.
 * Jeżeli alokacja pamięci się nie powiedzie, zwraca NULL.
 * @param ptr - Wskaźnik do użycia.
 * @return Nowy element do ArrayList, zawierający kopię tekstu text lub NULL.
 */
ArrayListNode * newArrayListNodeVoid(void * ptr);

/**
 * Tworzy nowy element tablicy ArrayList. Kopiuje stringa @p text do nowego elementu tablicowego.
 * Jeżeli alokacja pamięci się nie powiedzie, zwraca NULL.
 * @param text - Tekst jako char*.
 * @return Nowy element do ArrayList, zawierający kopię tekstu text lub NULL.
 */
ArrayListNode* newArrayListNode(char const * text);

/**
 * Zwraca @p idx-ty (indeksowany od 0) element tablicy lub NULL jeżeli @p al jest NULLem lub @p idx jest poza rozmiarem tablicy.
 * @param al - wskaźnik na tablicę
 * @param idx - pozycja, z której funkcja odzyskuje wartość
 * @return Wskaźnik na zawartość idx-tego elementu tablicy lub NULL.
 */
void const * arrayListGet (ArrayList const * al, size_t idx);

/**
 * [ADAPTER (arrayListGet)] Zwraca wskaźnik na @p idx-ty (indeksowany od 0) element tablicy @p pnum.
 * @param pnum - wskaźnik na tablicę
 * @param idx - pozycja w tablicy al
 * @return Wskaźnik na zawartość idx-tego elementu tablicy @p pnum. Jeżeli tablica jest NULLem lub rozmiar tablicy <= @p idx, to zwraca NULL.
 */
char const * phnumGet (PhoneNumbers const * pnum, size_t idx);

/**
 * Dodaje element @p node na koniec tablicy @p al
 * @param al - wskaźnik na tablicę
 * @param node - wskaźnik na element do dodania
 * @return Informacja, czy dodanie się powiodło. W szczególności false, jeżeli tablica lub element jest NULLem.
 */
bool arrayListAdd (ArrayList* al, ArrayListNode * node);

/**
 * Usuwa tablicę
 * @param al - tablica do usunięcia
 */
void arrayListDelete (ArrayList const * al);

/**
 * [ADAPTER] Usuwa tablicę
 * @param pnum - tablica do usunięcia
 */
void phnumDelete (PhoneNumbers const * pnum);

/**
 * Usuwa arrayListNode z tablicy @p al, nie zachowuje kolejności. Dealokuje @p aln. Jeżeli @p al jest NULLem, nic nie robi.
 * Jeżeli @p aln nie należy do @p al, to undefined behaviour.
 * @param al - tablica
 * @param aln - node do usunięcia
 */
void arrayListCutOut (ArrayList * al, ArrayListNode* aln);

/**
 * @param al - tablica do sprawdzenia
 * @return True jeżeli @p al jest NULLem lub pusta, false otherwise
 */
bool isArrayListEmpty (ArrayList * al);

/**
 * Sortuje tablicę ArrayNode'ów z char* qsortem. Nic nie robi, gdy tablica jest NULLem.
 * @param al - tablica do posortowania
 */
void stringArrayListSort (ArrayList * al);
