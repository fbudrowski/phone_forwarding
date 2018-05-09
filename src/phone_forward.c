

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "arrayList.h"


const size_t DIGIT_COUNT = 10;


/**
 * Struktura przechowująca przekierowania numerów telefonów.
 */
typedef struct PhoneForward {

    char digit; /** Cyfra w obecnym wierzchołku */

    struct PhoneForward * parent; /** Ojciec wierzchołka */
    struct PhoneForward ** forwards; /** Tablica synów */

    struct PhoneForward * redirTarget; /** Wskaźnik na przekierowanie numeru */
    ArrayListNode * redirNode; /** Wskaźnik na wierzchołek przekierowania */
    ArrayList* reverseRedirs; /** Lista wierzchołków, które przekierowują na obecny */


} PhoneForward;

/**
 * Sprawdza, czy ciąg znaków składa się z cyfr
 * @param number - ciąg znaków,
 * @return true jeżeli number składa się tylko z cyfr, false w przeciwnym wypadku.
 */

bool verifyNumber(char const * number){
  if (number == NULL) {
    return false;
  }
  while (*number > 0){
    if (*number < '0' || '9' < *number){
      return false;
    }
    number++;
  }
  return true;
}

PhoneForward* phfwdNew(void){
  PhoneForward* phFwd = malloc(sizeof(PhoneForward));

  if (phFwd == NULL){
    return NULL;
  }

  phFwd->digit = 0;

  phFwd->parent = NULL;
  phFwd->forwards = calloc(DIGIT_COUNT, sizeof(PhoneForward const *));

  phFwd->redirTarget = NULL;
  phFwd->redirNode = NULL;
  phFwd->reverseRedirs = NULL;
  return phFwd;
}

void phfwdDelete(PhoneForward* pf){
  if (pf == NULL){
    return;
  }

  if (pf->forwards != NULL) {
    for (size_t i = 0; i < DIGIT_COUNT; i++) {
      phfwdDelete(pf->forwards[i]);
    }
    free(pf->forwards);
  }
  arrayListDelete(pf->reverseRedirs);

  free(pf);
}
/**
 * Zwraca liczbę określoną cyfrą typu char
 * @param c - cyfra
 * @return wartość c w zakresie [0, 9];
 */
size_t charToDig(char const c){
  return (size_t)(c - '0');
}


/**
 * Oblicza liczbę synów wierzchołka prefiksowego.
 * @param pf - wierzchołek prefixowy
 * @return Liczba synów (rozszerzeń prefiksu) pf
 */
size_t countActiveSons(PhoneForward * pf){
  size_t activeSonsCount = 0;

  if (pf != NULL && pf->forwards != NULL){
    for (size_t i = 0; i < DIGIT_COUNT; i++){
      if (pf->forwards[i] != NULL){
        activeSonsCount++;
      }
    }
  }
  return activeSonsCount;
}

/**
 * Próbuje usunąć nieużywany wierzchołek prefixowy
 * @param pf - wierzchołek prefixowy do potencjalnego usunięcia
 */
void cleanNonRecursive(PhoneForward *pf){
  if (pf == NULL || pf->digit == 0 || isArrayListEmpty(pf->reverseRedirs) == false){
    return;
  }

  size_t activeSonsCount = countActiveSons(pf);

  if (activeSonsCount == 0){
    PhoneForward* parent = pf->parent;
    parent->forwards[charToDig(pf->digit)] = NULL;
    phfwdDelete(pf);
  }

}


/**
 * Próbuje usunąć nieużywany wierzchołek prefixowy i jego potencjalnie nieużywanych przodków
 * @param pf - najniższy wierzchołek do usunięcia
 */

void cleanDownToUpRecursive(PhoneForward *pf){
  if (pf == NULL || pf->digit == 0 || isArrayListEmpty(pf->reverseRedirs) == false){
    return;
  }

  size_t activeSonsCount = countActiveSons(pf);

  if (activeSonsCount == 0){
    PhoneForward* parent = pf->parent;
    pf->parent->forwards[pf->digit - '0'] = NULL;
    phfwdDelete(pf);
    cleanDownToUpRecursive(parent);
  }

}

/**
 * Zwraca najdalszy istniejący wierzchołek na ścieżce od korzenia do wierzchołka odpowiadającego numerowi prefix.
 * Nie tworzy nowych wierzchołków.
 * @param pf - wierzchołek początkowy (korzeń)
 * @param prefix - ciąg początkowy
 * @return Najdalszy wierzchołek na ścieżce do prefix. Jeżeli ciąg nie składa się tylko z cyfr, UB.
 */
PhoneForward* getVertexFromPrefixPassively (PhoneForward *pf, char const *prefix){

  if (prefix == NULL || *prefix == '\0'){
    return pf;
  }
  if (*prefix < '0' || '9' < *prefix){
    return NULL;
  }
  if (pf == NULL || pf->forwards == NULL || pf->forwards[charToDig(*prefix)] == NULL) {
    return pf;
  }

  return getVertexFromPrefixPassively(pf->forwards[charToDig(*prefix)], prefix + 1);
}

/**
 *
 * Zwraca wierzchołek odpowiadający numerowi prefix. W tym celu może tworzyć nowe wierzchołki. Przy błędzie alokacji, nie pozostawia dodatkowych
 * wierzchołków.
 * @param pf - wierzchołek początkowy
 * @param prefix - ciąg początkowy
 * @return Wierzchołek na ścieżce do prefix. NULL jeżeli wystąpił błąd alokacji.
 */
PhoneForward* getVertexFromPrefixAggressively(PhoneForward *pf, char const *prefix){
  if (prefix == NULL || *prefix == '\0') {
    return pf;
  }
  if (*prefix < '0' || '9' < *prefix){
    return NULL;
  }
  if (pf->forwards == NULL){
    pf->forwards = calloc(DIGIT_COUNT, sizeof(PhoneForward*));
    if (pf->forwards == NULL){
      cleanDownToUpRecursive(pf);
      return NULL;
    }
  }
  if (pf->forwards[charToDig(*prefix)] == NULL) {
    PhoneForward *newForward = phfwdNew();
    if (newForward == NULL){
      cleanDownToUpRecursive(pf);
      return NULL;
    }
    newForward->parent = pf;
    newForward->digit = *prefix;
    pf->forwards[charToDig(*prefix)] = newForward;
  }
  return getVertexFromPrefixAggressively(pf->forwards[charToDig(*prefix)], prefix + 1);
}

/**
 * Usuwa przekierowanie.
 * @param source - źródło usuwanego przekierowania, nie jest NULLem
 */
void removeTarget(PhoneForward *source){
  PhoneForward* target = source->redirTarget;

  if (target != NULL){
    ArrayListNode* toRemove = source->redirNode;

    arrayListCutOut(target->reverseRedirs, toRemove);
    cleanDownToUpRecursive(target);

    source->redirNode = NULL;
    source->redirTarget = NULL;
  }
}

bool phfwdAdd(PhoneForward* pf, char const * oldPrefix, char const * newPrefix){
  if (pf == NULL) {
    return false;
  }
  if (!verifyNumber(oldPrefix) || !verifyNumber(newPrefix)){
    return false;
  }

  PhoneForward* source = getVertexFromPrefixAggressively(pf, oldPrefix);

  if (source == NULL){
    return false;
  }

  removeTarget(source);

  PhoneForward* target = getVertexFromPrefixAggressively(pf, newPrefix);

  if (target == NULL){
    return false;
  }

  ArrayListNode* transferNode = malloc(sizeof(ArrayListNode));

  if (transferNode == NULL){
    return false;
  }

  transferNode->valueToBeFreed = false;
  transferNode->pos = 0;
  transferNode->value = source;
  if (target->reverseRedirs == NULL){
    target->reverseRedirs = newArrayList();
  }
  arrayListAdd(target->reverseRedirs, transferNode);

  source->redirNode = transferNode;
  source->redirTarget = target;


  return true;
}
/**
 * Usuwa rekurencyjnie (w dół) przekierowania.
 * @param pf - wierzchołek prefixowy określający początek przekierowania
 */

void removeRedirsRecursive(PhoneForward* pf) {
  if (pf == NULL) return;
  removeTarget(pf);

  if (pf->forwards != NULL) {
    for (size_t i = 0; i < DIGIT_COUNT; i++) {
      PhoneForward *son = pf->forwards[i];
      removeRedirsRecursive(son);
      cleanNonRecursive(son);
    }
  }
}

/**
 * Zwraca odległość wierzchołka pf od korzenia.
 * @param pf - wierzchołek, którego odległość sprawdzamy
 * @return odległość pf od korzenia
 */
size_t getDistance(PhoneForward* pf){
  if (pf == NULL || pf->digit == 0){
    return 0;
  }
  return getDistance(pf->parent) + 1;
}

/**
 * Zwraca pełny prefix (jako char*) reprezentujący odpowiedni wierzchołek w drzewie prefixowym.
 * @param pf - obecny wierzchołek
 * @param dist - liczba określająca liczbę wierzchołków niżej, inicjalizowana jako 0
 * @param current - wskaźnik do pozycji, na której ma być dokonany zapis. Proszę nie wrzucać tam NULLa.
 * @return wskaźnik na prefix.
 */

char * getFullPrefix(PhoneForward* pf, char * ptr0){
  if (pf->digit == 0){
    return ptr0;
  }
  char * prefix = getFullPrefix(pf->parent, ptr0);
  *prefix = pf->digit;
  prefix++;
  return prefix;
}

void phfwdRemove(PhoneForward* pf, char const * num){
  if (!verifyNumber(num)) {
    return;
  }
  PhoneForward * pf0 = getVertexFromPrefixAggressively(pf, num);
  removeRedirsRecursive(pf0);
  cleanDownToUpRecursive(pf0);
}

/**
 * Zwraca numer, na który przekierowany zostaje num.
 * @param pf - korzeń struktury
 * @param num - ciąg cyfr, wybrany numer
 * @return przekierowanie z num
 */
char * getRedirection(PhoneForward *pf, char const *num){

  PhoneForward * source = getVertexFromPrefixPassively(pf, num);

  while (source != NULL && source->redirTarget == NULL){
    source = source->parent;
  }
  size_t countReplacedLetters = 0;
  for (PhoneForward* pf1 = source; pf1 != pf && pf1 != NULL; pf1 = pf1->parent){
    countReplacedLetters++;
  }
  PhoneForward * target = (source == NULL) ? NULL : source->redirTarget;

  if (target == NULL){
    char * answer = calloc(strlen(num) + 1, sizeof(char));
    memcpy(answer, num, (strlen(num)) * sizeof(char));
    return answer;
  }

  size_t distance = getDistance(target);
  char * prefix = calloc(distance + 2, sizeof(char));
  getFullPrefix(target, prefix);


  size_t newPrefixLength = strlen(prefix);
  size_t oldNumberLength = strlen(num);
  size_t newNumberLength = newPrefixLength + oldNumberLength - countReplacedLetters;

  char* answer = calloc(newNumberLength + 2, sizeof(char));
  memcpy(answer, prefix, sizeof(char) * (newPrefixLength + 1));
  memcpy(answer + newPrefixLength, num + countReplacedLetters, sizeof(char) * (oldNumberLength-countReplacedLetters + 1));

  free(prefix);
  return answer;
}

PhoneNumbers const * phfwdGet(PhoneForward * pf, char const * num){
  if (!verifyNumber(num)){
    return NULL;
  }
  char * answer = getRedirection(pf, num);
  PhoneNumbers * pn = newArrayList();
  arrayListAdd(pn, newArrayListNode(answer));
  free(answer);
  return pn;
}


struct PhoneNumbers const * phfwdReverse(struct PhoneForward * pf, char const *num) {
  if (!verifyNumber(num)){
    return newArrayList();
  }

  ArrayList * answer = newArrayList();

  char* numRedirected = getRedirection(pf, num);
  if (strcmp(numRedirected, num) == 0){
    arrayListAdd(answer, newArrayListNode(numRedirected));
  }

  PhoneForward * target = getVertexFromPrefixAggressively(pf, num);
  PhoneForward * target0 = target;
  size_t numSize = strlen(num);
  char const * numPtr = num + numSize;
  size_t remaining = 0;

  while (target != NULL){
    if (target->reverseRedirs != NULL) {
      for (size_t i = 0; i < target->reverseRedirs->phNumCount; i++){
        PhoneForward * source = (PhoneForward *) target->reverseRedirs->phNums[i]->value;
        size_t sourceLen = getDistance(source);

        char * stringToBeReverted = calloc(sourceLen + remaining + 2, sizeof(char));
        getFullPrefix(source, stringToBeReverted);
        memcpy(stringToBeReverted + sourceLen, numPtr, remaining * sizeof(char));

        char * revertedString = getRedirection(pf, stringToBeReverted);

        if (strcmp(revertedString, num) == 0){
          arrayListAdd(answer, newArrayListNode(stringToBeReverted));
        }
        //printf("REVERT (%s %s): %s -> %s\n", numPtr, num, stringToBeReverted, revertedString);

        free(revertedString);
        free(stringToBeReverted);
      }
    }
    remaining++;
    numPtr--;
    target = target->parent;
  }
  cleanDownToUpRecursive(target0);

  stringArrayListSort(answer);
  for (size_t i = 1; i < answer->phNumCount; i++){
    if (strcmp(answer->phNums[i]->value, answer->phNums[i-1]->value) == 0){
      free((char*)answer->phNums[i-1]->value);
      answer->phNums[i-1]->value = NULL;
    }
  }
  for (size_t i = 0; i < answer->phNumCount; i++){
    while(i < answer->phNumCount && answer->phNums[i]->value == NULL){
      arrayListCutOut(answer, answer->phNums[i]);
    }
  }

  return answer;

}

